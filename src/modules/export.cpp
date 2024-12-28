#include "wincpp/modules/export.hpp"

#include <format>

namespace wincpp::modules
{
    std::string module_t::export_t::name() const noexcept
    {
        return export_name;
    }

    std::uintptr_t module_t::export_t::address() const noexcept
    {
        return mod->address() + rva;
    }

    std::uint16_t module_t::export_t::ordinal() const noexcept
    {
        return ordinal_value;
    }

    std::shared_ptr< const module_t > module_t::export_t::module() const noexcept
    {
        return mod;
    }

    module_t::export_t::export_t(
        std::shared_ptr< const module_t > mod,
        const std::string_view name,
        const std::uintptr_t rva,
        const std::uint16_t ordinal ) noexcept
        : export_name( name ),
          rva( rva ),
          ordinal_value( ordinal ),
          mod( mod )
    {
    }

    std::string module_t::export_t::to_string() const
    {
        return std::format( "{0} -> {1} (0x{2:X})", mod->name(), name(), address() );
    }

    std::ostream &operator<<( std::ostream &os, const module_t::export_t &exp )
    {
        return os << exp.to_string();
    }
}  // namespace wincpp::modules