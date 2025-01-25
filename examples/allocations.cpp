#include <array>
#include <chrono>
#include <execution>
#include <iostream>
#include <print>
#include <thread>
#include <wincpp/patterns/scanner.hpp>
#include <wincpp/process.hpp>

using namespace wincpp;

int main()
{
    try
    {
        const auto& process = process_t::open( "RobloxPlayerBeta.exe" );

        if ( !process )
        {
            std::println( "[Error] Roblox is not open." );
            return 1;
        }

        const std::string value = "Hello!";

        const auto& allocation = process->memory_factory.allocate( value.size() + 1, memory::protection_flags_t::readwrite, true );

        allocation->write( 0, value );

        std::println( "[Info] Wrote \"{}\" to 0x{:x}.", value, allocation->address() );
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