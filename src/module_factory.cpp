#include "wincpp/module_factory.hpp"

#include <algorithm>

#include "wincpp/process.hpp"

namespace wincpp
{
    module_factory::module_factory( process_t* p ) noexcept : p( p )
    {
    }

    modules::module_list module_factory::modules() const
    {
        return modules::module_list( p );
    }

    modules::module_t module_factory::main_module() const
    {
        return ( *this )[ p->name() ];
    }

    core::result_t< modules::module_t > module_factory::fetch_module( const std::string_view name ) const noexcept
    {
        for ( const auto& entry : core::snapshot< core::snapshot_kind::module_t >::create( p->id() ) )
        {
            if ( entry.name == name.data() )
            {
                return modules::module_t( p->memory_factory, entry );
            }
        }

        return std::unexpected( core::error::from_user( core::user_error_type_t::module_not_found_t, "Failed to find module \"{}\"", name ) );
    }

    modules::module_t module_factory::operator[]( const std::string_view name ) const
    {
        std::string data( name );

        // Convert the string to lowercase.
        std::transform( data.begin(), data.end(), data.begin(), ::tolower );

        // If the data doesn't contain the extension, add it.
        if ( data.find( ".dll" ) == std::string::npos && data.find( ".exe" ) == std::string::npos )
            data.append( ".dll" );

        const auto result = fetch_module( data );

        if ( result.has_value() )
            return result.value();

        throw result.error();
    }
}  // namespace wincpp