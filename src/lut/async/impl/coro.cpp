#include "lut/async/stable.hpp"
#include "lut/async/impl/coro.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{

    Coro::Coro()
        : Context(1024*32)
        , _code()
    {

    }

    Coro::~Coro()
    {
        assert(!_code);
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
                Thread::current()->scheduler()->coroEntry_deactivateAndStayEmpty(this);
            }
        }
    }

}}}
