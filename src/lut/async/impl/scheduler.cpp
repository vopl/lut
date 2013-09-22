#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{
    Scheduler::Scheduler()
        : _threadsMtx()
        , _threads()
    {
    }

    Scheduler::~Scheduler()
    {
        assert(_threads.empty());
    }

    ThreadReleaseResult Scheduler::threadRelease(const std::thread::id &id)
    {
        std::lock_guard<std::mutex> l(_threadsMtx);
        TMThreads::iterator iter = _threads.find(id);

        if(_threads.end() == iter)
        {
            return ThreadReleaseResult::notInWork;
        }

        iter->second->releaseRequest();
        return ThreadReleaseResult::ok;
    }

    bool Scheduler::threadEntry_init(Thread *thread)
    {
        assert(Thread::instance() == thread);

        std::lock_guard<std::mutex> l(_threadsMtx);

        std::pair<TMThreads::iterator, bool> insertRes =
            _threads.insert(std::make_pair(std::this_thread::get_id(), thread));

        return insertRes.second;
    }

    bool Scheduler::threadEntry_utilize(Thread *thread)
    {
        assert(Thread::instance() == thread);

#ifndef NDEBUG
        {
            std::lock_guard<std::mutex> l(_threadsMtx);
            assert(_threads.end() != _threads.find(std::this_thread::get_id()));
        }
#endif

        Coro *coro = _coroListReady.dequeue();
        if(!coro)
        {
            return false;
        }

        assert(Thread::instance() == thread);

        coro->activate();
        return true;
    }

    bool Scheduler::threadEntry_deinit(Thread *thread)
    {
        assert(Thread::instance() == thread);

        std::lock_guard<std::mutex> l(_threadsMtx);

        TMThreads::iterator iter = _threads.find(std::this_thread::get_id());

        if(_threads.end() == iter)
        {
            return false;
        }

        _threads.erase(iter);
        return true;
    }

    void Scheduler::spawn(const Task &code)
    {
        Coro *coro = _coroListEmpty.dequeue();
        if(!coro)
        {
            coro = new Coro();
        }

        coro->setCode(code);
        _coroListReady.enqueue(coro);
    }

    void Scheduler::spawn(Task &&code)
    {
        Coro *coro = _coroListEmpty.dequeue();
        if(!coro)
        {
            coro = new Coro();
        }

        coro->setCode(std::forward<Task>(code));
        _coroListReady.enqueue(coro);
    }

}}}
