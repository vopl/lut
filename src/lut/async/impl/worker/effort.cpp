#include "lut/async/stable.hpp"
#include "lut/async/impl/worker/effort.hpp"

namespace lut { namespace async { namespace impl { namespace worker
{

    Effort::Effort()
    {
    }

    Effort::~Effort()
    {
    }

    bool Effort::haveTasks()
    {
        return !_tasks.empty();
    }

    void Effort::moveTasksTo(Effort *target)
    {
        _tasks.moveTo(target->_tasks);
    }

    bool Effort::haveReadyCoros()
    {
        return !_readyCoros.empty();
    }

    void Effort::moveReadyCorosTo(Effort *target)
    {
        _readyCoros.moveTo(target->_readyCoros);
    }

    void Effort::enqueueTask(Task *task)
    {
        _tasks.enqueue(task);
    }

    ctx::Coro *Effort::dequeueReadyCoro()
    {
        ctx::Coro *coro = _readyCoros.dequeue();
        if(coro)
        {
            return coro;
        }

        if(!_tasks.empty())
        {
            coro = _emptyCoros.dequeue();
            if(!coro)
            {
                coro = ctx::Coro::alloc();
            }
            coro->setCode(_tasks.dequeue());
            return coro;
        }

        return nullptr;
    }


}}}}
