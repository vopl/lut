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

        Coro *coro = (Coro *)
                ((intptr_t)(stack->_userspaceBegin - sizeof(Coro)) & ~0xf);

        new(coro) Coro(stack);

        return coro;
    }

    Coro::Coro(const lut::mm::Stack *stack)
        : _stack(stack)
    {
        constructCoro(
                    _stack->_guardBegin,
                    &Coro::s_contextProc,
                    reinterpret_cast<intptr_t>(this));
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

    void Coro::s_contextProc(intptr_t self)
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
