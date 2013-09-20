#include "lut/async/stable.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{
    thread_local Thread *Thread::_thread(nullptr);

    Thread::Thread(Scheduler &scheduler, ThreadState *stateEvt)
        : _scheduler(scheduler)
        , _stateEvt(stateEvt)
        , _mtx()
        , _cv()
        , _releaseRequest(false)
        , _context()
    {
        assert(!_thread);
    }

    Thread::~Thread()
    {
        assert(!_thread);
        assert(!_context);
    }

    Thread *Thread::instance()
    {
        return _thread;
    }

    void Thread::releaseRequest()
    {
        std::lock_guard<std::mutex> l(_mtx);

        _releaseRequest = true;
        _cv.notify_all();
    }

}}}
