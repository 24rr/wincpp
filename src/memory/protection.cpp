#include "wincpp/memory/protection.hpp"

#include <sstream>

namespace wincpp::memory
{
    const protection_flags_t protection_flags_t::execute_readwrite{ protection_t::execute_readwrite_t };
    const protection_flags_t protection_flags_t::readwrite{ protection_t::readwrite_t };
    const protection_flags_t protection_flags_t::noaccess{ protection_t::noaccess_t };
    const protection_flags_t protection_flags_t::guard{ protection_t::guard_t };

    protection_flags_t::protection_flags_t( std::uint32_t flags ) noexcept : flags( flags )
    {
    }

    std::uint32_t protection_flags_t::get() const noexcept
    {
        return flags;
    }

    bool operator==( const protection_flags_t& lhs, const protection_flags_t& rhs )
    {
        return lhs.flags == rhs.flags;
    }

    std::ostream& operator<<( std::ostream& os, const protection_flags_t& flags )
    {
        std::ostringstream ss;

        if ( flags.has( protection_t::noaccess_t ) )
            ss << "noaccess | ";

        if ( flags.has( protection_t::readonly_t ) )
            ss << "readonly | ";

        if ( flags.has( protection_t::readwrite_t ) )
            ss << "readwrite | ";

        if ( flags.has( protection_t::writecopy_t ) )
            ss << "writecopy | ";

        if ( flags.has( protection_t::execute_t ) )
            ss << "execute | ";

        if ( flags.has( protection_t::execute_read_t ) )
            ss << "execute_read | ";

        if ( flags.has( protection_t::execute_readwrite_t ) )
            ss << "execute_readwrite | ";

        if ( flags.has( protection_t::execute_writecopy_t ) )
            ss << "execute_writecopy | ";

        if ( flags.has( protection_t::guard_t ) )
            ss << "guard | ";

        if ( flags.has( protection_t::nocache_t ) )
            ss << "nocache | ";

        if ( flags.has( protection_t::writecombine_t ) )
            ss << "writecombine | ";

        if ( flags.has( protection_t::targets_invalid_t ) )
            ss << "tarets_invalid | ";

        if ( flags.has( protection_t::targets_no_update_t ) )
            ss << "targets_no_update | ";

        auto str = ss.str();

        if ( !str.empty() )
            str = str.substr( 0, str.size() - 3 );

        return os << str;
    }

}  // namespace wincpp::memory