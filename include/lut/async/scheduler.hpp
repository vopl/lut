#ifndef _LUT_ASYNC_SCHEDULER_HPP_
#define _LUT_ASYNC_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/hiddenImpl.hpp"

#include <thread>
#include <functional>

namespace lut { namespace async
{
    namespace impl
    {
        class Scheduler;
    }

    class Scheduler
        : private HiddenImpl<impl::Scheduler>
    {
        Scheduler(const Scheduler &from) = delete;
        Scheduler &operator=(const Scheduler &from) = delete;

    public:
        Scheduler();
        ~Scheduler();

    public://threads
        ThreadUtilizationResult threadUtilize(ThreadState *stateEvt = NULL);
        ThreadReleaseResult threadRelease();
        ThreadReleaseResult threadRelease(std::thread::native_handle_type id);

    public://code
        void spawn(const std::function<void()> &code);
        void spawn(std::function<void()> &&code);
    };
}}

#endif
