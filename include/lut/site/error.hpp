#pragma once

#include <system_error>

namespace lut { namespace site { namespace error
{
    enum class general
    {
        unknown = 1,
        already_runned,
        stopping_in_progress,
        modules_directory_absent,
        failed,
    };

    const std::error_category& general_category();

}}}

namespace std
{
    template<>
    struct is_error_code_enum<lut::site::error::general>
        : public true_type { };
}

namespace lut { namespace site
{
    std::error_code make_error_code(error::general e);
}}
