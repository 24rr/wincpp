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

        const auto& kernel32 = process->module_factory[ "kernel32.dll" ];

        for ( const auto& exp : kernel32.exports() )
        {
            std::println( "[+] Export: {}", exp->to_string() );
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