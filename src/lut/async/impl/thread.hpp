#ifndef _LUT_ASYNC_IMPL_THREAD_HPP_
#define _LUT_ASYNC_IMPL_THREAD_HPP_

#include "lut/async/scheduler.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/context.hpp"
#include "lut/async/impl/coro.hpp"
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
        Context *getRootContext();
        Scheduler *getScheduler();

    public:
        void storeEmptyCoro(Coro *coro);
        void storeReadyCoro(Coro *coro);

        Coro *fetchEmptyCoro();
        Coro *fetchReadyCoro();

    public:
        void setCurrentCoro(Coro *coro);
        Coro *getCurrentCoro();


    private:
        std::thread::id             _id;

        Scheduler                   *_scheduler;
        ThreadState                 *_stateEvt;

        std::atomic_bool            _releaseRequest;

        static thread_local Thread  *_current;
        Context                     *_rootContext;

        Coro                        *_storedEmptyCoro;
        Coro                        *_storedReadyCoro;

        Coro                        *_currentCoro;

    private:
        friend class Scheduler;
        CoroContainer               _coroListReady;
    };
}}}

#endif
