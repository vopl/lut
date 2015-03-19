
#include <loop.hpp>
#include "impl/loop.hpp"

namespace dci { namespace io { namespace loop
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
