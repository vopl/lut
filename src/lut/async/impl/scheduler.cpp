#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/ctx/stackAllocator.hpp"
#include "lut/async/impl/ctx/coro.hpp"

#include <algorithm>

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::Scheduler()
        : _currentCoro(nullptr)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::~Scheduler()
    {
        ctx::Coro *coro = _emptyCoros.dequeue();
        while(coro)
        {
            coro->free();
            coro = _emptyCoros.dequeue();
        }
    }

    void Scheduler::spawn(Task *task)
    {
        _tasks.enqueue(task);
    }

    void Scheduler::yield()
    {
        assert(_currentCoro);
        ctx::Coro *coro = _currentCoro;
        _readyCoros.enqueue(coro);

        ctx::Coro *nextCoro = dequeueReadyCoro();
        while(nextCoro)
        {
            if(nextCoro == coro)
            {
                return;
            }

            _currentCoro = nextCoro;
            coro->switchTo(nextCoro);
            nextCoro = dequeueReadyCoro();
        }

        _currentCoro = nullptr;
        coro->switchTo(&_rootContext);
    }

    void Scheduler::utilize()
    {
        assert(!_currentCoro);

        ctx::Coro *nextCoro = dequeueReadyCoro();
        while(nextCoro)
        {
            _currentCoro = nextCoro;
            _rootContext.switchTo(nextCoro);
            nextCoro = dequeueReadyCoro();
        }
    }

    void Scheduler::coroCompleted()
    {
        assert(_currentCoro);
        ctx::Coro *coro = _currentCoro;
        _emptyCoros.enqueue(coro);

        ctx::Coro *nextCoro = dequeueReadyCoro();
        while(nextCoro)
        {
            if(nextCoro == coro)
            {
                return;
            }

            _currentCoro = nextCoro;
            coro->switchTo(nextCoro);
            nextCoro = dequeueReadyCoro();
        }

        _currentCoro = nullptr;
        coro->switchTo(&_rootContext);
    }

    ctx::Coro *Scheduler::dequeueReadyCoro()
    {
        ctx::Coro *coro = _readyCoros.dequeue();
        if(coro)
        {
            return coro;
        }

        Task *task = _tasks.dequeue();

        if(task)
        {
            coro = _emptyCoros.dequeue();
            if(!coro)
            {
                coro = ctx::Coro::alloc(this);
            }
            coro->setCode(task);
            return coro;
        }

        return nullptr;
    }

}}}
