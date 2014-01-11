#ifndef _LUT_ASYNC_IMPL_WORKER_THREAD_HPP_
#define _LUT_ASYNC_IMPL_WORKER_THREAD_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/worker/effort.hpp"

#include <thread>
#include <atomic>


namespace lut { namespace async { namespace impl
{
    class Scheduler;
}}}

namespace lut { namespace async { namespace impl { namespace worker
{
    class Thread
    {
    public:
        Thread(Scheduler *scheduler, ThreadState *stateEvt);
        ~Thread();

        ThreadUtilizationResult utilize();

    private:
        std::thread::id             _id;

        Scheduler                   *_scheduler;
        ThreadState                 *_stateEvt;

        std::atomic_bool            _releaseRequest;

        static __thread Thread      *_current;
        ctx::Root                   *_rootContext;

        Effort                      _effort;
    };
}}}}

#endif
