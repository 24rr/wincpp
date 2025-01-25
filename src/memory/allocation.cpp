#include "wincpp/memory/allocation.hpp"

namespace wincpp::memory
{
    void allocation_t::deleter::operator()( allocation_t* allocation ) const noexcept
    {
        if ( allocation->_owns )
            allocation->factory.free( allocation->address() );

        delete allocation;
    }

    allocation_t::allocation_t( const memory_factory& mem, std::uintptr_t address, std::size_t size, bool owns ) noexcept
        : memory_t( mem, address, size ),
          _owns( owns )
    {
    }
}  // namespace wincpp::memory