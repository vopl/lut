#pragma once

#include <system_error>

namespace lut { namespace module { namespace error
{
    enum class general
    {
        unknown = 1,
        module_already_attached,
        module_busy,
        wrong_module_state,
        unable_load_binary,
        malformed_module_binary,
        bad_info_file,
    };

    const std::error_category& general_category();

}}}

namespace std
{
    template<>
    struct is_error_code_enum<lut::module::error::general>
        : public true_type { };
}

namespace lut { namespace module
{
    std::error_code make_error_code(error::general e);
}}
