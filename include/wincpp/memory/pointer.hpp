#pragma once

#include <concepts>
#include <iostream>

#include "memory.hpp"

namespace wincpp::memory
{
    /// <summary>
    /// A class representing a pointer in the memory of the remote process.
    /// </summary>
    template< typename T >
    struct pointer_t : public memory_t
    {
        /// <summary>
        /// A class representing a value in the memory of the remote process.
        /// </summary>
        template< typename U >
        struct value_t final
        {
            /// <summary>
            /// Creates a new value object.
            /// </summary>
            /// <param name="address">The address of the value.</param>
            /// <param name="factory">The process's memory factory.</param>
            explicit value_t( std::uintptr_t address, const memory_factory &factory ) noexcept : address( address ), factory( factory )
            {
            }

            /// <summary>
            /// Converts the value to the type T.
            /// </summary>
            /// <returns>The value as type T.</returns>
            inline operator T() const
            {
                return factory.read< T >( address );
            }

            /// <summary>
            /// Assigns a value to the memory.
            /// </summary>
            inline const value_t &operator=( T value ) const
            {
                factory.write< T >( address, value );
                return *this;
            }

            /// <summary>
            /// Gets the values as a pointer.
            /// </summary>
            inline operator pointer_t< U >() const
            {
                return pointer_t< U >( factory.read< T >( address ), factory );
            }

            /// <summary>
            /// Writes the value of the pointer to the output stream.
            /// </summary>
            /// <param name="os">The output stream.</param>
            /// <param name="value">The value object.</param>
            /// <returns>The output stream.</returns>
            friend std::ostream &operator<<( std::ostream &os, const value_t &value )
            {
                os << static_cast< T >( value );
                return os;
            }

            /// <summary>
            /// Compares the value with another value.
            /// </summary>
            /// <param name="lhs">The left-hand side value.</param>
            /// <param name="rhs">The right-hand side value.</param>
            /// <returns>True if the values are equal, false otherwise.</returns>
            friend bool operator==( const value_t &lhs, const T &rhs )
            {
                return static_cast< T >( lhs ) == rhs;
            }

            /// <summary>
            /// Compares the value with another value.
            /// </summary>
            /// <param name="lhs">The left-hand side value.</param>
            /// <param name="rhs">The right-hand side value.</param>
            /// <returns>True if the values are equal, false otherwise.</returns>
            friend bool operator==( const T &lhs, const value_t &rhs )
            {
                return lhs == static_cast< T >( rhs );
            }

            /// <summary>
            /// Compares the value with another value.
            /// </summary>
            /// <param name="lhs">The left-hand side value.</param>
            /// <param name="rhs">The right-hand side value.</param>
            /// <returns>True if the values are not equal, false otherwise.</returns>
            friend bool operator!=( const value_t &lhs, const T &rhs )
            {
                return static_cast< T >( lhs ) != rhs;
            }

            /// <summary>
            /// Compares the value with another value.
            /// </summary>
            /// <param name="lhs">The left-hand side value.</param>
            /// <param name="rhs">The right-hand side value.</param>
            /// <returns>True if the values are not equal, false otherwise.</returns>
            friend bool operator!=( const T &lhs, const value_t &rhs )
            {
                return lhs != static_cast< T >( rhs );
            }

            friend bool operator==( const value_t &lhs, const value_t &rhs )
            {
                return static_cast< T >( lhs ) == static_cast< T >( rhs );
            }

            friend bool operator!=( const value_t &lhs, const value_t &rhs )
            {
                return static_cast< T >( lhs ) != static_cast< T >( rhs );
            }

            std::uintptr_t address;
            memory_factory factory;
        };

        /// <summary>
        /// Creates a new pointer object.
        /// </summary>
        /// <param name="address">The address of the pointer.</param>
        /// <param name="factory">The process's memory factory.</param>
        explicit pointer_t( const std::uintptr_t address, const memory_factory &factory ) noexcept
            : memory_t( factory, address, sizeof( T ) ),
              value( address, factory )
        {
        }

        template< typename U >
        inline operator pointer_t< U >() const
        {
            return pointer_t< U >( value.address, value.factory );
        }

        /// <summary>
        /// Dereferences the pointer.
        /// </summary>
        /// <returns>The value at the pointer.</returns>
        inline value_t< T > operator*() const noexcept
        {
            return value;
        }

        /// <summary>
        /// Converts the pointer to the address.
        /// </summary>
        /// <returns>The address of the pointer.</returns>
        inline operator std::uintptr_t() const noexcept
        {
            return value.address;
        }

        /// <summary>
        /// Returns whether the pointer is valid.
        /// </summary>
        /// <remarks>
        /// Todo: Search for a memory region that contains the pointer. If the region is not found, the pointer is invalid.
        /// </remarks>
        inline operator bool() const noexcept
        {
            if ( value.address != 0 )
            {
                for ( const auto &region : value.factory.regions() )
                {
                    if ( region.contains( value.address ) )
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        /// <summary>
        /// Increments the pointer by the specified offset.
        /// </summary>
        /// <param name="offset">The offset to increment the pointer by.</param>
        template< std::integral U >
        inline pointer_t< T > operator+( U offset ) const noexcept
        {
            return pointer_t< T >( value.address + offset, value.factory );
        }

        /// <summary>
        /// Writes the value of the pointer to the output stream.
        /// </summary>
        /// <param name="os">The output stream.</param>
        /// <param name="pointer">The pointer object.</param>
        /// <returns>The output stream.</returns>
        friend std::ostream &operator<<( std::ostream &os, const pointer_t &pointer )
        {
            os << "0x" << std::hex << pointer.value.address;
            return os;
        }

        /// <summary>
        /// Compares the pointer with another pointer.
        /// </summary>
        /// <param name="lhs">The left-hand side pointer.</param>
        /// <param name="rhs">The right-hand side pointer.</param>
        /// <returns>True if the pointers are equal, false otherwise.</returns>
        friend bool operator==( const pointer_t &lhs, const pointer_t &rhs )
        {
            return lhs.value.address == rhs.value.address;
        }

        /// <summary>
        /// Compares the pointer with another pointer.
        /// </summary>
        /// <param name="lhs">The left-hand side pointer.</param>
        /// <param name="rhs">The right-hand side pointer.</param>
        /// <returns>True if the pointers are not equal, false otherwise.</returns>
        friend bool operator!=( const pointer_t &lhs, const pointer_t &rhs )
        {
            return lhs.value.address != rhs.value.address;
        }

        value_t< T > value;
    };
}  // namespace wincpp::memory