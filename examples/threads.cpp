#include <chrono>
#include <execution>
#include <iostream>
#include <thread>
#include <print>
#include <wincpp/patterns/scanner.hpp>
#include <wincpp/process.hpp>

using namespace wincpp;

int main()
{
    try
    {
        const auto process = process_t::open( "RobloxPlayerBeta.exe" );

        if ( !process )
        {
            std::cout << "Failed to open the process." << std::endl;
            return 1;
        }

        const auto& main_module = process->module_factory.main_module();

        std::unordered_map< std::uintptr_t, std::shared_ptr< modules::module_t::export_t > > exports;

        for ( const auto& module : process->module_factory.modules() )
        {
            for ( const auto& e : module->exports() )
            {
                exports[ e->address() ] = e;
            }
        }

        // Get the .rdata section.
        if ( const auto& rdata = main_module.fetch_section( ".rdata" ) )
        {
            // Read the entire section.
            const auto& buffer = rdata->read();

            // Iterate over each address in the section.
            for ( std::size_t i = 0; i < rdata->size(); i += sizeof( std::uintptr_t ) )
            {
                const auto& address = *reinterpret_cast< std::uintptr_t* >( buffer.get() + i );

                // If the address is an export, print it.
                if ( const auto& e = exports.find( address ); e != exports.end() )
                {
                    std::println( "{} @ IAT 0x{:X}", e->second->to_string(), rdata->address() + i );
                }
            }
        }
    }
    catch ( const std::system_error& e )
    {
        std::cout << "[-] Error [" << e.code() << "]: " << e.what() << std::endl;
    }
    catch ( const std::exception& e )
    {
        std::cout << "[-] Error: " << e.what() << std::endl;
    }

    return 0;
}