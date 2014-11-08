#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/ctx/coro.hpp"

#include <algorithm>

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::Scheduler()
        : _currentCoro(nullptr)
    {
        uv_loop_init(&_uv_loop);
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

        uv_loop_close(&_uv_loop);
    }

    Scheduler &Scheduler::instance()
    {
        return _instance;
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

        switchToNext();
    }

    void Scheduler::hold()
    {
        assert(_currentCoro);

        switchToNext();
    }

    void Scheduler::ready(ctx::Coro *coro)
    {
        _readyCoros.enqueue(coro);
    }

    void Scheduler::run()
    {
        executeReadyCoros();

        uv_check_t  uv_check;
        uv_check_init(&_uv_loop, &uv_check);

        uv_check.data = this;
        uv_check_start(&uv_check, [](uv_check_t *handle)
        {
            static_cast<Scheduler *>(handle->data)->executeReadyCoros();
        });

        uv_run(&_uv_loop, UV_RUN_DEFAULT);

        uv_check_stop(&uv_check);
    }

    void Scheduler::stop()
    {
        uv_stop(&_uv_loop);
    }

    void Scheduler::executeReadyCoros()
    {
        assert(!_currentCoro);

        ctx::Coro *next = dequeueReadyCoro();
        if(next)
        {
            _currentCoro = next;
            _rootContext.switchTo(next);
        }
    }

    ctx::Coro *Scheduler::currentCoro()
    {
        return _currentCoro;
    }

    void Scheduler::coroCompleted()
    {
        assert(_currentCoro);
        ctx::Coro *coro = _currentCoro;
        _emptyCoros.enqueue(coro);

        switchToNext();
    }

    void Scheduler::switchToNext()
    {
        ctx::Coro *current = _currentCoro;
        ctx::Coro *next = dequeueReadyCoro();
        if(next)
        {
            if(next == current)
            {
                return;
            }

            _currentCoro = next;
            current->switchTo(next);
        }
        else
        {
            _currentCoro = nullptr;
            current->switchTo(&_rootContext);
        }
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

                if(!coro)
                {
                    return nullptr;
                }
            }
            coro->setCode(task);
            return coro;
        }

        return nullptr;
    }

    Scheduler Scheduler::_instance{};
}}}
