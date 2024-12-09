#include <wincpp/process.hpp>
#include <wincpp/threads/thread.hpp>

namespace wincpp::threads
{
    thread_t::thread_t( const core::thread_entry_t& entry ) : entry( entry )
    {
    }

    const std::shared_ptr< core::handle_t > thread_t::handle() const
    {
        if ( hthread == nullptr )
        {
            const auto thread = OpenThread( THREAD_ALL_ACCESS, FALSE, entry.id );

            if ( !thread )
                throw core::error::from_win32( GetLastError() );

            hthread = core::handle_t::create( thread );
        }

        return hthread;
    }

    thread_list::iterator thread_list::begin() const noexcept
    {
        auto it = snapshot.begin();

        while ( it != snapshot.end() && ( *++it ).owner_id != process->id() )
            ;

        return iterator( process, it );
    }

    thread_list::iterator thread_list::end() const noexcept
    {
        return iterator( process, snapshot.end() );
    }

    std::vector< thread_t > thread_list::vector() const
    {
        std::vector< thread_t > threads;

        for ( auto it = begin(); it != end(); ++it )
            threads.push_back( *it );

        return threads;
    }

    thread_list::thread_list( process_t* process ) noexcept
        : snapshot( core::snapshot< core::snapshot_kind::thread_t >::create() ),
          process( process )
    {
    }

    thread_list::iterator::iterator( process_t* process, const core::snapshot< core::snapshot_kind::thread_t >::iterator& it ) noexcept
        : process( process ),
          it( it )
    {
    }

    thread_t thread_list::iterator::operator*() const noexcept
    {
        return thread_t{ *it };
    }

    thread_list::iterator& thread_list::iterator::operator++()
    {
        while ( it != core::snapshot< core::snapshot_kind::thread_t >::iterator{ nullptr } && ( *++it ).owner_id != process->id() )
            ;

        return *this;
    }

    bool thread_list::iterator::operator==( const iterator& other ) const noexcept
    {
        return it == other.it;
    }

    bool thread_list::iterator::operator!=( const iterator& other ) const noexcept
    {
        return !operator==( other );
    }

}  // namespace wincpp::threads