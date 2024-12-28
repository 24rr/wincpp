#include <chrono>
#include <execution>
#include <iostream>
#include <thread>
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

        const auto& hyp = process->module_factory[ "RobloxPlayerBeta.dll" ];

        // 48 8D 0D ? ? ? ? 48 8D 55 F8 -> lea rcx, [rel data_????????]
        const auto& address = hyp->find( patterns::pattern_t{ "\x48\x8D\x0D\x00\x00\x00\x00\x48\x8D\x55\xF8", "xxx????xxxx" } );

        if ( !address )
        {
            std::cout << "Failed to find the pattern." << std::endl;
            return 1;
        }

        std::cout << "Found the pattern at: 0x" << std::hex << *address << std::endl;

        const auto& instruction = hyp->read( *address, 7 );

        const auto& relative_offset = *reinterpret_cast< std::int32_t* >( instruction.get() + 3 );
        const auto& data_address = *address + 7 + relative_offset;

        std::cout << "The data address is: 0x" << std::hex << data_address << ", 0x" << data_address - hyp->address() << std::endl;

        const auto& kernel32 = process->module_factory[ "kernel32.dll" ];

        std::cout << "0x" << kernel32->address() << std::endl;
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