#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

namespace wincpp::threads
{
    struct thread_t;
    class thread_list;
}  // namespace wincpp::threads

namespace wincpp
{
    /// <summary>
    /// The factory that creates and manages threads.
    /// </summary>
    class thread_factory final
    {
        friend struct process_t;

        process_t *p;

        /// <summary>
        /// Creates a new window factory object.
        /// </summary>
        /// <param name="process">The process object.</param>
        explicit thread_factory( process_t *p ) noexcept;

       public:
        /// <summary>
        /// Gets a list of threads in the process.
        /// </summary>
        /// <returns>The list of threads.</returns>
        threads::thread_list threads() const;

        /// <summary>
        /// Suspends all threads in the process.
        /// </summary>
        void suspend_all() const;

        /// <summary>
        /// Resumes all threads in the process.
        /// </summary>
        void resume_all() const;

        /// <summary>
        /// Gets the thread by its id.
        /// </summary>
        /// <param name="index">The id of the thread.</param>
        /// <returns>The thread.</returns>
        std::optional< threads::thread_t > fetch_thread( std::size_t index ) const;

        /// <summary>
        /// Gets the thread by its id.
        /// </summary>
        /// <param name="index">The id of the thread.</param>
        /// <returns>The thread.</returns>
        threads::thread_t operator[]( std::size_t index ) const;
    };
}  // namespace wincpp

#include <wincpp/threads/thread.hpp>