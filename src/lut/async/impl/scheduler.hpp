#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include <thread>

namespace lut { namespace async { namespace impl
{

    class Scheduler
    {
    public:
        Scheduler();
        ~Scheduler();

        ThreadReleaseResult threadRelease();
        ThreadReleaseResult threadRelease(std::thread::native_handle_type id);

    };
}}}

#endif
