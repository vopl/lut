#ifndef _LUT_ASYNC_IMPL_WORKER_HPP_
#define _LUT_ASYNC_IMPL_WORKER_HPP_

#include "lut/async/scheduler.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include "lut/async/impl/coroContainer.hpp"

namespace lut { namespace async { namespace impl
{
    class Worker
    {
    public:
        Worker(Scheduler *scheduler, ThreadState *stateEvt);
        ~Worker();

        ThreadUtilizationResult utilize();

    private:
        std::thread::id             _id;

        Scheduler                   *_scheduler;
        ThreadState                 *_stateEvt;

        std::atomic_bool            _releaseRequest;

        static __thread Worker      *_current;
        ctx::Root                   *_rootContext;

        CoroContainer               _corosEmpty;
        CoroContainer               _corosReady;
        TaskContainer               _tasksReady;
    };
}}}

#endif
