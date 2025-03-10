#include "wincpp/process.hpp"

#include <psapi.h>
#include <algorithm>
#include <cctype>
#include <string>

#include "wincpp/core/snapshot.hpp"

namespace wincpp
{
    bool case_insensitive_equal(std::string_view a, std::string_view b)
    {
        if (a.size() != b.size())
            return false;
            
        return std::equal(a.begin(), a.end(), b.begin(), 
            [](char a, char b) {
                return std::tolower(static_cast<unsigned char>(a)) == 
                       std::tolower(static_cast<unsigned char>(b));
            });
    }

    std::unique_ptr< process_t > process_t::open( std::string_view name, std::uint32_t access )
    {
        for ( const auto& proc : core::snapshot< core::snapshot_kind::process_t >::create() )
        {
            if ( case_insensitive_equal(proc.name, name) )
            {
                const auto handle = OpenProcess( access, FALSE, proc.id );

                if ( !handle )
                    throw core::error::from_win32( GetLastError() );

                return std::unique_ptr< process_t >( new process_t( core::handle_t::create( handle ), proc, memory_type::remote_t ) );
            }
        }

        return nullptr;
    }

    std::unique_ptr< process_t > process_t::open( std::uint32_t id, std::uint32_t access )
    {
        for ( const auto& proc : core::snapshot< core::snapshot_kind::process_t >::create() )
        {
            if ( proc.id == id )
            {
                const auto handle = OpenProcess( access, FALSE, id );

                if ( !handle )
                    throw core::error::from_win32( GetLastError() );

                return std::unique_ptr< process_t >( new process_t( core::handle_t::create( handle ), proc, memory_type::remote_t ) );
            }
        }

        return nullptr;
    }

    std::unique_ptr< process_t > process_t::current()
    {
        const auto handle = core::handle_t::create( GetCurrentProcess(), false );

        core::process_entry_t entry{};

        entry.id = GetProcessId( handle->native );

        if ( !GetModuleBaseName( handle->native, nullptr, entry.name.data(), MAX_PATH ) )
            throw core::error::from_win32( GetLastError() );

        return std::unique_ptr< process_t >( new process_t( handle, entry, memory_type::local_t ) );
    }

    process_t::process_t( std::shared_ptr< core::handle_t > handle, const core::process_entry_t& entry, memory_type type ) noexcept
        : handle( handle ),
          entry( entry ),
          module_factory( this ),
          memory_factory( this, type ),
          window_factory( this ),
          thread_factory( this )
    {
    }

}  // namespace wincpp