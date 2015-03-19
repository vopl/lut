#pragma once

#include "task.hpp"
#include "ctx/root.hpp"
#include "ctx/coro.hpp"
#include "effortContainer.hpp"

namespace dci { namespace async { namespace impl
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
