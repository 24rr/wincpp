#pragma once

#include "module.hpp"
#include "wincpp/memory/memory.hpp"

namespace wincpp::modules
{
    /// <summary>
    /// Class representing a section of a module.
    /// </summary>
    struct module_t::section_t : public memory::memory_t
    {
        friend struct module_t;

        /// <summary>
        /// Gets the name of the section.
        /// </summary>
        std::string_view name() const noexcept;

       private:
        /// <summary>
        /// Creates a new section object.
        /// </summary>
        /// <param name="mod">The mod object.</param>
        /// <param name="header">The section header.</param>
        explicit section_t( std::shared_ptr< const module_t > mod, const IMAGE_SECTION_HEADER& header ) noexcept;

        std::shared_ptr< const module_t > mod;
        IMAGE_SECTION_HEADER header;

        std::size_t name_size;
    };
}  // namespace wincpp::modules