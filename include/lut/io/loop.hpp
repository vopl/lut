#pragma once

#include <system_error>

namespace lut { namespace io { namespace loop
{

    std::error_code run();
    std::error_code stop();

}}}
