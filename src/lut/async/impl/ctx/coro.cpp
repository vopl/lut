#include "lut/stable.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include "lut/mm/functions.hpp"
#include "lut/async/impl/scheduler.hpp"

namespace lut { namespace async { namespace impl { namespace ctx
{
    Coro *Coro::alloc(Scheduler *scheduler)
    {
        const lut::mm::Stack *stack = mm::stackAlloc();
        if(!stack)
        {
            return nullptr;
        }

        Coro *coro;
        if(stack->_stackGrowsDown)
        {
            coro = reinterpret_cast<Coro *>(stack->_userspaceEnd - sizeof(Coro));
        }
        else
        {
            coro = reinterpret_cast<Coro *>(stack->_userspaceBegin);
        }

        new(coro) Coro(scheduler, stack);

        return coro;
    }

    Coro::Coro(Scheduler *scheduler, const lut::mm::Stack *stack)
        : _scheduler(scheduler)
        , _stack(stack)
    {
        if(stack->_stackGrowsDown)
        {
            char *end = reinterpret_cast<char *>(this);
            constructCoro(
                        _stack->_userspaceBegin,
                        end - _stack->_userspaceBegin,
                        &Coro::s_contextProc,
                        reinterpret_cast<std::intptr_t>(this));
        }
        else
        {
            char *begin = reinterpret_cast<char *>(this) + sizeof(Coro);
            constructCoro(
                        begin,
                        _stack->_userspaceEnd - begin,
                        &Coro::s_contextProc,
                        reinterpret_cast<std::intptr_t>(this));
        }
    }

    void Coro::free()
    {
        const lut::mm::Stack *stack = _stack;
        this->~Coro();

        mm::stackFree(stack);
    }

    Coro::~Coro()
    {
        destructCoro();
        //_stack = NULL;
    }

    void Coro::setCode(Task *task)
    {
        assert(!_task);
        _task = task;
        assert(_task);
    }

    void Coro::switchTo(Engine *to)
    {
        mm::stackCompact(_stack);
        Engine::switchTo(to);
    }

    void Coro::s_contextProc(std::intptr_t self)
    {
        reinterpret_cast<Coro*>(self)->contextProc();
    }

    void Coro::contextProc()
    {
        for(;;)
        {
            assert(_task);

            try
            {
                _task->call();
            }
            catch(const std::exception &e)
            {
                std::cerr<<"std exception catched in contextProc: "<<e.what();
                abort();
            }
            catch(...)
            {
                std::cerr<<"unknown exception catched in contextProc";
                abort();
            }

            assert(_task);
            {
                _task->free();
                _task = nullptr;
            }

            _scheduler->coroCompleted();
        }
    }

}}}}
