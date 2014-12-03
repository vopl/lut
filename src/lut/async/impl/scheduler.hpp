#pragma once

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
        static Scheduler &instance();

    public:
        void spawn(Task *task);
        void yield();
        void hold();
        void ready(ctx::Coro *coro);
        void executeReadyCoros();

    public:
        ctx::Coro *currentCoro();
        void coroCompleted();

    private:
        void switchToNext();
        ctx::Coro *dequeueReadyCoro();

    private:
        static Scheduler    _instance;
        ctx::Root           _rootContext;
        ctx::Coro           *_currentCoro;

        EffortContainer<ctx::Coro>  _emptyCoros;
        EffortContainer<ctx::Coro>  _readyCoros;
        EffortContainer<Task>       _tasks;
    };
}}}
