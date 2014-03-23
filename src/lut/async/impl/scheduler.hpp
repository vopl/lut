#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/impl/task.hpp"
#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include "lut/async/impl/effortContainer.hpp"


namespace lut { namespace async { namespace impl
{
    class Scheduler
    {
    public:
        Scheduler();
        ~Scheduler();

    public:
        void spawn(Task *task);
        void yield();
        void utilize();

    public:
        void coroCompleted();

    private:
        ctx::Coro *dequeueReadyCoro();

    private:
        ctx::Root           _rootContext;
        ctx::Coro           *_currentCoro;

        EffortContainer<ctx::Coro>  _emptyCoros;
        EffortContainer<ctx::Coro>  _readyCoros;
        EffortContainer<Task>       _tasks;

    };
}}}

#endif
