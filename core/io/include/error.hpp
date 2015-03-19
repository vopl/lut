#pragma once

#include <system_error>

namespace dci { namespace io { namespace error
{
    enum class general
    {
        already_runned = 1,
        not_runned,
        invalid_argument,
        secondary_listen,
        no_listen,
        not_implemented,
        unknown,
    };

    enum class stream
    {
        not_connected = 1,
        closed,
    };

    const std::error_category& general_category();
    const std::error_category& stream_category();

}}}

namespace std
{
    template<>
    struct is_error_code_enum<dci::io::error::general>
        : public true_type { };

    template<>
    struct is_error_code_enum<dci::io::error::stream>
        : public true_type { };
}

namespace dci { namespace io
{
    std::error_code make_error_code(error::general e);
    std::error_code make_error_code(error::stream e);
}}
