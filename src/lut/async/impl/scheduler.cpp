#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/worker/thread.hpp"

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::Scheduler()
        : _threadsMtx()
        , _threads()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::~Scheduler()
    {
        assert(_threads.empty());
    }

    ThreadReleaseResult Scheduler::releaseThread(const std::thread::id &id)
    {
        assert(0);
    }

    bool Scheduler::threadEntry_init(Thread *thread)
    {
        assert(0);
    }

    bool Scheduler::threadEntry_deinit(Thread *thread)
    {
        assert(0);
    }

    void Scheduler::spawn(Task *code)
    {
        assert(0);
    }

    void Scheduler::yield()
    {
        assert(0);
    }

}}}
