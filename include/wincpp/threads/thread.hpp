#pragma once

#include <vector>
#include <wincpp/core/snapshot.hpp>
#include <wincpp/core/win.hpp>

namespace wincpp
{
    class thread_factory;
    struct process_t;
}  // namespace wincpp

namespace wincpp::threads
{
    class thread_list;

    /// <summary>
    /// Represents a thread object. Can be remote or local.
    /// </summary>
    struct thread_t
    {
        friend class thread_list;
        friend class thread_factory;

        /// <summary>
        /// Gets the handle to the thread.
        /// </summary>
        /// <returns>The handle to the thread.</returns>
        const std::shared_ptr< core::handle_t > handle() const;

        /// <summary>
        /// Gets the id of the thread.
        /// </summary>
        /// <returns>The id of the thread.</returns>
        constexpr std::uint32_t id() const noexcept
        {
            return entry.id;
        }

        /// <summary>
        /// Gets the process id of the thread.
        /// </summary>
        /// <returns>The process id of the thread.</returns>
        constexpr std::uint32_t process_id() const noexcept
        {
            return entry.owner_id;
        }

        /// <summary>
        /// Suspends the thread.
        /// </summary>
        /// <returns>The previous suspend count.</returns>
        std::size_t suspend() const;

        /// <summary>
        /// Resumes the thread.
        /// </summary>
        /// <returns>The previous suspend count.</returns>
        std::size_t resume() const;

        /// <summary>
        /// Gets the thread context.
        /// </summary>
        /// <param name="flag">The context flags.</param>
        /// <returns>The thread context.</returns>
        CONTEXT context( std::uint64_t flag = CONTEXT_ALL ) const;

       protected:
        /// <summary>
        /// Creates a new thread object.
        /// </summary>
        /// <param name="type">The type of thread.</param>
        explicit thread_t( const core::thread_entry_t& entry );

        core::thread_entry_t entry;
        mutable std::shared_ptr< core::handle_t > hthread;
    };

    /// <summary>
    /// Represents a list of threads in the remote process. Contains the iterator for ToolHelp32Snapshot.
    /// </summary>
    class thread_list final
    {
        friend class thread_factory;

        core::snapshot< core::snapshot_kind::thread_t > snapshot;
        process_t* process;

        /// <summary>
        /// Creates a new thread list object.
        /// </summary>
        /// <param name="process">The process object.</param>
        explicit thread_list( process_t* process ) noexcept;

       public:
        /// <summary>
        /// The iterator for the thread list.
        /// </summary>
        class iterator;

        /// <summary>
        /// Gets the begin iterator for the thread list.
        /// </summary>
        iterator begin() const noexcept;

        /// <summary>
        /// Gets the end iterator for the thread list.
        /// </summary>
        iterator end() const noexcept;

        /// <summary>
        /// Gets the threadlist as a vector.
        /// </summary>
        /// <returns></returns>
        std::vector< thread_t > vector() const;
    };

    class thread_list::iterator final
    {
        friend class thread_list;

        core::snapshot< core::snapshot_kind::thread_t >::iterator it;
        process_t* process;

        /// <summary>
        /// Creates a new thread iterator object.
        /// </summary>
        /// <param name="it">The thread iterator.</param>
        iterator( process_t* process, const core::snapshot< core::snapshot_kind::thread_t >::iterator& it ) noexcept;

       public:
        /// <summary>
        /// Gets the thread object.
        /// </summary>
        /// <returns>The thread object.</returns>
        thread_t operator*() const noexcept;

        /// <summary>
        /// Moves to the next thread.
        /// </summary>
        /// <returns>The next thread iterator.</returns>
        iterator& operator++();

        /// <summary>
        /// Compares two iterators.
        /// </summary>
        /// <param name="other">The other iterator.</param>
        /// <returns>Whether the iterators are equal.</returns>
        bool operator==( const iterator& other ) const noexcept;

        /// <summary>
        /// Compares two iterators.
        /// </summary>
        /// <param name="other">The other iterator.</param>
        /// <returns>Whether the iterators are not equal.</returns>
        bool operator!=( const iterator& other ) const noexcept;
    };
}  // namespace wincpp::threads