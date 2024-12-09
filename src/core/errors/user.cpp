#include <wincpp/core/errors/user.hpp>

namespace wincpp::core
{
    const user_error_category& user_error_category::get() noexcept
    {
        static user_error_category instance;
        return instance;
    }

    const char* user_error_category::name() const noexcept
    {
        return "user";
    }

    std::string user_error_category::message( int code ) const
    {
        const auto& c = static_cast< user_error_type_t >( code );

        switch ( c )
        {
            case user_error_type_t::module_not_found_t: return "The desired module was not found.";
            case user_error_type_t::thread_not_found_t: return "The desired thread was not found.";
            default: return "Unknown error";
        }
    }
}  // namespace wincpp::core