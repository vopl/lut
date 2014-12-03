#include "lut/stable.hpp"
#include "lut/io/loop.hpp"
#include "lut/io/impl/loop.hpp"

#include <sys/epoll.h>

namespace lut { namespace io { namespace loop
{

    std::error_code run()
    {
        return impl::loop::run();
    }

    std::error_code stop()
    {
        return impl::loop::stop();
    }

}}}
