#ifndef _LUT_ASYNC_IMPL_THREAD_HPP_
#define _LUT_ASYNC_IMPL_THREAD_HPP_

#include "lut/async/scheduler.hpp"

namespace lut { namespace async { namespace impl
{
    class Thread
    {
    public:
        Thread(Scheduler &scheduler, ThreadState *stateEvt);
        ~Thread();

        template <class StoppingMeter>
        ThreadUtilizationResult utilize(StoppingMeter &stoppingMeter);

    private:
        Scheduler &_scheduler;
        ThreadState *_stateEvt;

        std::mutex _mtx;
        std::condition_variable _cv;

        bool _releaseRequest;
    };

    template <class StoppingMeter>
    ThreadUtilizationResult Thread::utilize(StoppingMeter &stoppingMeter)
    {
        assert(0);
        return ThreadUtilizationResult::limitExhausted;
    }

}}}

#endif
