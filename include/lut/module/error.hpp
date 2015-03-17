#pragma once

#include <system_error>

namespace lut { namespace module { namespace error
{
    enum class general
    {
        unknown = 1,
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
