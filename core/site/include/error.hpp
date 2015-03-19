#pragma once

#include <system_error>

namespace dci { namespace site { namespace error
{
    enum class general
    {
        unknown = 1,
        already_started,
        already_stopped,
        starting_in_progress,
        stopping_in_progress,
        modules_directory_absent,
        failed,
        parial_failed,
    };

    enum class module
    {
        unknown = 1,
        busy,
        wrong_state,
        unable_load_binary,
        malformed_binary,
        bad_info_file,
    };

    const std::error_category& general_category();
    const std::error_category& module_category();

}}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::site::error::general>
        : public true_type { };

    template<>
    struct is_error_code_enum<dci::site::error::module>
        : public true_type { };
}

namespace dci { namespace site
{
    std::error_code make_error_code(error::general e);
    std::error_code make_error_code(error::module e);
}}
