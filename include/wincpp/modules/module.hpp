#pragma once

// clang-format off
#include "wincpp/core/snapshot.hpp"
#include "wincpp/memory/memory.hpp"
#include "wincpp/modules/object.hpp"
// clang-format on

#include <Psapi.h>

#include <list>
#include <memory>
#include <optional>
#include <vector>

namespace wincpp
{
    struct process_t;
    class module_factory;
}  // namespace wincpp

namespace wincpp::modules
{
    class module_list;

    /// <summary>
    /// Class representing a module in a process.
    /// </summary>
    struct module_t : public std::enable_shared_from_this< module_t >, memory::memory_t
    {
        friend class module_list;
        friend class module_factory;

        /// <summary>
        /// Represents an export of a module.
        /// </summary>
        struct export_t;

        /// <summary>
        /// Represents a section of a module.
        /// </summary>
        struct section_t;

        /// <summary>
        /// Gets the name of the module.
        /// </summary>
        std::string_view name() const noexcept;

        /// <summary>
        /// Gets the memory address of the entry point of the module.
        /// </summary>
        std::uintptr_t entry_point() const noexcept;

        /// <summary>
        /// Gets the full path to the module.
        /// </summary>
        std::string path() const noexcept;

        /// <summary>
        /// Gets the list of exports in the module.
        /// </summary>
        const std::list< std::shared_ptr< module_t::export_t > > &exports() const noexcept;

        /// <summary>
        /// Gets the export by its name.
        /// </summary>
        /// <param name="name">The name of the export.</param>
        /// <returns>The export.</returns>
        std::shared_ptr< module_t::export_t > fetch_export( const std::string_view name ) const;

        /// <summary>
        /// Gets the list of sections in the module.
        /// </summary>
        const std::list< std::shared_ptr< module_t::section_t > > &sections() const noexcept;

        /// <summary>
        /// Gets the section by its name.
        /// </summary>
        /// <param name="name">The name of the section.</param>
        /// <returns>The section.</returns>
        std::shared_ptr< module_t::section_t > fetch_section( const std::string_view name ) const;

        /// <summary>
        /// Locates all objects in the module by their mangled name.
        /// </summary>
        /// <param name="mangled">The mangled name.</param>
        /// <returns>A list of objects.</returns>
        std::vector< std::shared_ptr< rtti::object_t > > fetch_objects( const std::string_view mangled ) const;

        /// <summary>
        /// Gets the export by its name.
        /// </summary>
        const export_t& operator[]( const std::string_view name ) const;

       private:
        /// <summary>
        /// Creates a new module object.
        /// </summary>
        /// <param name="factory">The memory factory.</param>
        /// <param name="entry">The module entry.</param>
        explicit module_t( const memory_factory &factory, const core::module_entry_t &entry ) noexcept;

        core::module_entry_t entry;
        MODULEINFO info;

        const IMAGE_DOS_HEADER *dos_header;
        const IMAGE_NT_HEADERS *nt_headers;

        mutable std::list< std::shared_ptr< module_t::export_t > > _exports;
        mutable std::list< std::shared_ptr< module_t::section_t > > _sections;
    
        std::shared_ptr< std::uint8_t[] > buffer;
    };
}  // namespace wincpp::modules