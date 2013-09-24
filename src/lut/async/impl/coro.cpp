#include "lut/async/stable.hpp"
#include "lut/async/impl/coro.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{
    Coro *Coro::make(size_t stackSize)
    {
        Coro *coro = (Coro *)malloc(sizeof(Coro) + stackSize);

        new(coro) Coro;

        coro->_engine.constructCoro(
                    sizeof(ContextEngine) + stackSize,
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

    void Coro::setCode(const Task &code)
    {
        assert(!_code);
        _code = code;
        assert(_code);
    }

    void Coro::setCode(Task &&code)
    {
        assert(!_code);
        _code = code;
        assert(_code);
    }

    void Coro::switchTo(Context *to)
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
            Thread *thread = Thread::current();
            thread->scheduler()->enqueuePerThreadCoros(thread);
        }

        for(;;)
        {
            assert(_code);

            try
            {
                _code();
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

            assert(_code);
            {
                Task().swap(_code);
            }

            {
                Thread::current()->scheduler()->coroEntry_stayEmptyAndDeactivate(this);
            }
        }
    }
}}}
