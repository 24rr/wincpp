#pragma once

// clang-format off
#include "core/win.hpp"
#include "core/snapshot.hpp"
// clang-format on

#include <wincpp/core/error.hpp>
#include <unordered_set>

namespace wincpp::modules
{
    /// <summary>
    /// Forward declaration of module_t.
    /// </summary>
    struct module_t;

    /// <summary>
    /// Forward declaration of module_list.
    /// </summary>
    class module_list;

}  // namespace wincpp::modules

namespace wincpp
{
    struct process_t;

    /// <summary>
    /// Class providing tools for manipulating modules and libraries.
    /// </summary>
    class module_factory final
    {
        friend struct process_t;

        process_t *p;

        mutable std::vector< std::shared_ptr< modules::module_t > > module_list;
        mutable std::unordered_set< std::uintptr_t > module_keys;

        /// <summary>
        /// Creates a new module factory object.
        /// </summary>
        /// <param name="process">The process object.</param>
        explicit module_factory( process_t *p ) noexcept;

       public:
        /// <summary>
        /// Gets a list of modules in the process.
        /// </summary>
        /// <returns>The list of modules.</returns>
        const std::vector< std::shared_ptr< modules::module_t > >& modules() const;

        /// <summary>
        /// Gets the main module of the process.
        /// </summary>
        const modules::module_t& main_module() const;

        /// <summary>
        /// Gets a module by its name.
        /// </summary>
        /// <param name="name">The name of the module.</param>
        /// <returns>The module.</returns>
        std::shared_ptr< modules::module_t > fetch_module( const std::string_view name ) const noexcept;

        /// <summary>
        /// Gets a module by its name.
        /// </summary>
        const modules::module_t& operator[]( const std::string_view name ) const;
    };
}  // namespace wincpp

#include "modules/export.hpp"
#include "modules/module.hpp"
#include "modules/object.hpp"
#include "modules/section.hpp"
