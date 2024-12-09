#pragma once

#include <expected>
#include <format>
#include <system_error>
#include <wincpp/core/errors/user.hpp>
#include <wincpp/core/errors/win32.hpp>

namespace wincpp::core
{
    /// <summary>
    /// Represents an error returned by the Windows API.
    /// </summary>
    class error : public std::system_error
    {
        using std::system_error::system_error;

       public:
        /// <summary>
        /// Creates a new error object with the given error code.
        /// </summary>
        static error from_win32( std::uint32_t code ) noexcept;

        /// <summary>
        /// Creates a new error object with the given user-defined error and formatted message.
        /// </summary>
        template< typename... T >
        static error from_user( const user_error_type_t& code, const std::format_string< T... > format, T&&... args ) noexcept
        {
            const auto& user_error_code = std::error_code( static_cast< int >( code ), user_error_category::get() );

            try
            {
                return error( user_error_code, std::format( format, std::forward< T >( args )... ) );
            }
            catch ( const std::exception& )
            {
                return error( user_error_code );
            }
        }
    };

    /// <summary>
    /// The result type for the Windows API.
    /// </summary>
    template< typename T >
    using result_t = std::expected< T, error >;

    /// <summary>
    /// The unexpected type for the Windows API.
    /// </summary>
    using unexpected_t = std::unexpected< error >;

}  // namespace wincpp::core