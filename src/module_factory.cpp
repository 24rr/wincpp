#include "wincpp/module_factory.hpp"

#include <algorithm>

#include "wincpp/process.hpp"

namespace wincpp
{
    module_factory::module_factory( process_t* p ) noexcept : p( p )
    {
    }

    const std::vector< std::shared_ptr< modules::module_t > >& module_factory::modules() const
    {
        std::unordered_set< std::uintptr_t > modules_encountered;

        for ( const auto& entry : core::snapshot< core::snapshot_kind::module_t >::create( p->id() ) )
        {
            if ( module_keys.find( entry.base_address ) == module_keys.end() )
            {
                module_list.emplace_back( new modules::module_t( p->memory_factory, entry ) );
                module_keys.insert( entry.base_address );
            }

            modules_encountered.insert( entry.base_address );
        }

        // Remove any modules that are no longer in the process.
        for ( auto it = module_list.begin(); it != module_list.end(); )
        {
            if ( modules_encountered.find( ( *it )->address() ) == modules_encountered.end() )
                it = module_list.erase( it );
            else
                ++it;
        }

        return module_list;
    }

    const modules::module_t& module_factory::main_module() const
    {
        return this->operator[]( p->name() );
    }

    std::shared_ptr< modules::module_t > module_factory::fetch_module( const std::string_view name ) const noexcept
    {
        for ( const auto& m : modules() )
        {
            if ( m->name() == name )
                return m;
        }

        return nullptr;
    }

    const modules::module_t& module_factory::operator[]( const std::string_view name ) const
    {
        std::string data( name );

        // Convert the string to lowercase.
        std::transform( data.begin(), data.end(), data.begin(), ::tolower );

        // If the data doesn't contain the extension, add it.
        if ( data.find( ".dll" ) == std::string::npos && data.find( ".exe" ) == std::string::npos )
            data.append( ".dll" );

        if ( const auto result = fetch_module( data ) )
            return *result;

        throw core::error::from_user( core::user_error_type_t::module_not_found_t, "Failed to find module \"{}\"", name );
    }
}  // namespace wincpp