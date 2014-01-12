#ifndef _LUT_ASYNC_IMPL_WORKER_EFFORT_HPP_
#define _LUT_ASYNC_IMPL_WORKER_EFFORT_HPP_

#include "lut/async/impl/ctx/coro.hpp"
#include "lut/async/impl/task.hpp"
#include "lut/async/impl/worker/effortContainer.hpp"

namespace lut { namespace async { namespace impl { namespace worker
{
    class Effort
    {
    public:
        Effort();
        ~Effort();

        bool haveTasks();
        void moveTasksTo(Effort *target);

        bool haveReadyCoros();
        void moveReadyCorosTo(Effort *target);



        void enqueueTask(Task *task);

        ctx::Coro *dequeueReadyCoro();

    private:
        EffortContainer<ctx::Coro*>  _emptyCoros;
        EffortContainer<ctx::Coro*>  _readyCoros;
        EffortContainer<Task*>       _tasks;
    };
}}}}

#endif

