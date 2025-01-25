#pragma once

#include "wincpp/memory/memory.hpp"
#include "wincpp/memory/protection_operation.hpp"

namespace wincpp::memory
{
    /// <summary>
    /// Represents an allocation in the process.
    /// </summary>
    struct allocation_t : memory_t
    {
        friend class memory_factory;

        /// <summary>
        /// The deleter for the allocation.
        /// </summary>
        struct deleter
        {
            /// <summary>
            /// Frees the memory backing the allocation.
            /// </summary>
            void operator()( allocation_t* allocation ) const noexcept;
        };

       protected:
        /// <summary>
        /// Creates a new allocation.
        /// </summary>
        /// <param name="mem">The memory factory.</param>
        /// <param name="address">The address of the allocation.</param>
        /// <param name="size">The size of the allocation</param>
        explicit allocation_t( const memory_factory& mem, std::uintptr_t address, std::size_t size, bool owns = false ) noexcept;

        bool _owns;
    };
}  // namespace wincpp::memory