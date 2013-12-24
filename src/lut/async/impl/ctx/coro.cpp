#include "lut/async/stable.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include "lut/async/impl/stackAllocator.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl { namespace ctx
{
    Coro *Coro::alloc(bool quietFail)
    {
        const sm::Stack *stack = StackAllocator::instance().alloc(quietFail);
        if(!stack)
        {
            return nullptr;
        }

        Coro *coro = (Coro *)
                ((intptr_t)(stack->_userspaceBound - sizeof(Coro)) & ~0xf);

        new(coro) Coro(stack);

        return coro;
    }

    Coro::Coro(const lut::async::impl::sm::Stack *stack)
        : _stack(stack)
    {
        constructCoro(
                    _stack->_guardBound,
                    &Coro::s_contextProc,
                    reinterpret_cast<intptr_t>(this));
    }

    void Coro::free()
    {
        const sm::Stack *stack = _stack;
        this->~Coro();

        bool b = StackAllocator::instance().free(stack);
        assert(b);
        (void)b;
    }

    Coro::~Coro()
    {
        destructCoro();
        //_stack = NULL;
    }

    void Coro::setCode(Task &&code)
    {
        assert(_task.empty());
        code.moveTo(std::forward<Task>(_task));
        assert(!_task.empty());
    }

    void Coro::switchTo(Engine *to)
    {
        StackAllocator::instance().compact(_stack);
        Engine::switchTo(to);
    }

    void Coro::s_contextProc(intptr_t self)
    {
        reinterpret_cast<Coro*>(self)->contextProc();
    }

    void Coro::contextProc()
    {
        {
            Thread *thread = Thread::getCurrent();
            thread->getScheduler()->enqueuePerThreadCoros(thread);
        }

        for(;;)
        {
            assert(!_task.empty());

            try
            {
                _task.exec();
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

            assert(!_task.empty());
            {
                _task.clear();
            }

            {
                Thread *thread = Thread::getCurrent();
                thread->getScheduler()->coroEntry_stayEmptyAndDeactivate(this, thread);
            }
        }
    }
}}}}
