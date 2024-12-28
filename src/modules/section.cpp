#include "wincpp/modules/section.hpp"

#include "wincpp/process.hpp"

#ifdef max
#undef max
#endif  // max

namespace wincpp::modules
{
    module_t::section_t::section_t( std::shared_ptr< const module_t > mod, const IMAGE_SECTION_HEADER &header ) noexcept
        : memory_t( mod->factory, mod->address() + header.VirtualAddress, std::max( header.SizeOfRawData, header.Misc.VirtualSize ) ),
          mod( mod ),
          header( header )
    {
        for ( name_size = 0; name_size < sizeof( header.Name ); ++name_size )
        {
            if ( header.Name[ name_size ] == '\0' )
                break;
        }
    }

    std::string_view module_t::section_t::name() const noexcept
    {
        return std::string_view( reinterpret_cast< const char * >( header.Name ), name_size );
    }
}  // namespace wincpp::modules