#ifndef _LUT_ASYNC_IMPL_THREAD_HPP_
#define _LUT_ASYNC_IMPL_THREAD_HPP_

#include "lut/async/scheduler.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/context.hpp"

namespace lut { namespace async { namespace impl
{
    class Thread
    {
    public:
        Thread(Scheduler &scheduler, ThreadState *stateEvt);
        ~Thread();

        ThreadUtilizationResult utilize();

        static Thread *instance();
        Context *context();

    public:
        void releaseRequest();

    private:
        Scheduler &_scheduler;
        ThreadState *_stateEvt;

        std::mutex _mtx;
        std::condition_variable _cv;

        bool _releaseRequest;

        static thread_local Thread *_thread;
        Context *_context;
    };
}}}

#endif
