#ifndef _LUT_ASYNC_IMPL_THREAD_HPP_
#define _LUT_ASYNC_IMPL_THREAD_HPP_

#include "lut/async/scheduler.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include "lut/async/impl/coroContainer.hpp"

namespace lut { namespace async { namespace impl
{
    class Thread
    {
    public:
        Thread(Scheduler *getScheduler, ThreadState *stateEvt);
        ~Thread();

        ThreadUtilizationResult utilize();

    public:
        void releaseRequest();
        bool isReleaseRequested();

    public:
        std::thread::id getId();
        static Thread *getCurrent();
        ctx::Root *getRootContext();
        Scheduler *getScheduler();

    public:
        void storeEmptyCoro(ctx::Coro *coro);
        void storeReadyCoro(ctx::Coro *coro);

        ctx::Coro *fetchEmptyCoro();
        ctx::Coro *fetchReadyCoro();

    public:
        void setCurrentCoro(ctx::Coro *coro);
        ctx::Coro *getCurrentCoro();


    private:
        std::thread::id             _id;

        Scheduler                   *_scheduler;
        ThreadState                 *_stateEvt;

        std::atomic_bool            _releaseRequest;

        static thread_local Thread  *_current;
        ctx::Root                   *_rootContext;

        ctx::Coro                   *_storedEmptyCoro;
        ctx::Coro                   *_storedReadyCoro;

        ctx::Coro                   *_currentCoro;
    };
}}}

#endif
