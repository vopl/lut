#include "lut/async/stable.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl { namespace ctx
{
    Coro *Coro::make(size_t stackSize)
    {
        Coro *coro = (Coro *)malloc(sizeof(Coro) + stackSize);

        new(coro) Coro;

        coro->_engine.constructCoro(
                    sizeof(Engine) + stackSize,
                    &Coro::s_contextProc,
                    reinterpret_cast<intptr_t>(coro));

        return coro;
    }

    Coro::Coro()
    {
    }

    void Coro::operator delete(void *p)
    {
        free(p);
    }

    Coro::~Coro()
    {
        _engine.destructCoro();
    }

    void Coro::setCode(Task &&code)
    {
        assert(_task.empty());
        code.moveTo(std::forward<Task>(_task));
        assert(!_task.empty());
    }

    void Coro::switchTo(Root *to)
    {
        _engine.switchTo(&to->_engine);
    }

    void Coro::switchTo(Coro *to)
    {
        _engine.switchTo(&to->_engine);
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
