#pragma once

#include "lut/io/impl/fd/base.hpp"
#include <system_error>

namespace lut { namespace io { namespace impl { namespace loop
{

    std::error_code listenerAdd(fd::Base *fdb);
    std::error_code listenerReplace(fd::Base *fdbFrom, fd::Base *fdbTo);
    std::error_code listenerDel(fd::Base *fdb);


    std::error_code run();
    std::error_code stop();

}}}}
