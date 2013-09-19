#ifndef _LUT_ASYNC_IMPL_THREADCONTAINER_HPP_
#define _LUT_ASYNC_IMPL_THREADCONTAINER_HPP_

#include "lut/async/scheduler.hpp"

namespace lut { namespace async { namespace impl
{

    class ThreadContainer
    {
    public:
        ThreadContainer();
        ~ThreadContainer();

        ThreadReleaseResult threadRelease();
        ThreadReleaseResult threadRelease(std::thread::native_handle_type id);

    };
}}}

#endif
