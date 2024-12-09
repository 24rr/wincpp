#include <wincpp/thread_factory.hpp>

namespace wincpp
{
    thread_factory::thread_factory( process_t* p ) noexcept : p( p )
    {
    }

    threads::thread_list thread_factory::threads() const
    {
        return threads::thread_list( p );
    }

    std::optional< threads::thread_t > thread_factory::fetch_thread( std::size_t index ) const
    {
        for ( const auto& thread : threads() )
        {
            if ( thread.id() == index )
                return thread;
        }

        return std::nullopt;
    }

    threads::thread_t thread_factory::operator[]( std::size_t index ) const
    {
        if ( const auto result = fetch_thread( index ) )
            return *result;

        throw core::error::from_user( core::user_error_type_t::thread_not_found_t, "Failed to find thread with id {}", index );
    }  
}  // namespace wincpp