#include <system_error>

namespace wincpp::core
{
    /// <summary>
    /// The different types of user-defined errors.
    /// </summary>
    enum class user_error_type_t
    {
        /// <summary>
        /// The desired module was not found.
        /// </summary>
        module_not_found_t
    };

    /// <summary>
    /// The error category for user-defined errors (errors that wincpp creates).
    /// </summary>
    struct user_error_category : public std::error_category
    {
        /// <summary>
        /// Returns the error category for user errors.
        /// </summary>
        static const user_error_category& get() noexcept;

        /// <summary>
        /// Returns the name of the error category.
        /// </summary>
        const char* name() const noexcept override;

        /// <summary>
        /// Returns the error message for the given error code.
        /// </summary>
        std::string message( int code ) const override;
    };

}  // namespace wincpp::core