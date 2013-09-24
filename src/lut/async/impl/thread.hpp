#ifndef _LUT_ASYNC_IMPL_THREAD_HPP_
#define _LUT_ASYNC_IMPL_THREAD_HPP_

#include "lut/async/scheduler.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/context.hpp"
#include "lut/async/impl/coro.hpp"

namespace lut { namespace async { namespace impl
{
    class Thread
    {
    public:
        Thread(Scheduler *scheduler, ThreadState *stateEvt);
        ~Thread();

        ThreadUtilizationResult utilize();

    public:
        void releaseRequest();
        bool isReleaseRequested();

    public:
        static Thread *current();
        Context *context();
        Scheduler *scheduler();

    public:
        void storeEmptyCoro(Coro *coro);
        void storeReadyCoro(Coro *coro);

        Coro *fetchEmptyCoro();
        Coro *fetchReadyCoro();

    public:
        void setCurrentCoro(Coro *coro);
        Coro *getCurrentCoro();


    private:
        Scheduler                   *_scheduler;
        ThreadState                 *_stateEvt;

        std::atomic_bool            _releaseRequest;

        static thread_local Thread  *_current;
        Context                     *_context;

        Coro                        *_storedEmptyCoro;
        Coro                        *_storedReadyCoro;

        Coro                        *_currentCoro;
    };
}}}

#endif
