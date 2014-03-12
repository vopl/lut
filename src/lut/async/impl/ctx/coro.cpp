#include "lut/async/stable.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include "lut/async/impl/ctx/stackAllocator.hpp"

namespace lut { namespace async { namespace impl { namespace ctx
{
    Coro *Coro::alloc()
    {
        const lut::mm::Stack *stack = StackAllocator::instance().stackAlloc();
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

        new(coro) Coro(stack);

        return coro;
    }

    Coro::Coro(const lut::mm::Stack *stack)
        : _stack(stack)
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

        StackAllocator::instance().stackFree(stack);
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
        StackAllocator::instance().stackCompact(_stack);
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
        }
    }

}}}}
