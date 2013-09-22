#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/thread.hpp"

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
        while(Coro *empty = _coroListEmpty.dequeue())
        {
            delete empty;
        }
        while(Coro *ready = _coroListReady.dequeue())
        {
            delete ready;
        }

        assert(_threads.empty());
    }

    ////////////////////////////////////////////////////////////////////////////////
    ThreadReleaseResult Scheduler::threadRelease(const std::thread::id &id)
    {
        std::lock_guard<std::mutex> l(_threadsMtx);
        TMThreads::iterator iter = _threads.find(id);

        if(_threads.end() == iter)
        {
            return ThreadReleaseResult::notInWork;
        }

        iter->second->releaseRequest();
        _threadsCv.notify_all();
        return ThreadReleaseResult::ok;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Scheduler::threadEntry_init(Thread *thread)
    {
        assert(Thread::current() == thread);

        std::lock_guard<std::mutex> l(_threadsMtx);

        std::pair<TMThreads::iterator, bool> insertRes =
            _threads.insert(std::make_pair(std::this_thread::get_id(), thread));

        return insertRes.second;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Scheduler::threadEntry_utilize(Thread *thread)
    {
        assert(Thread::current() == thread);

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

        assert(Thread::current() == thread);

        thread->context()->switchTo(coro);

        enqueuePerThreadCoros(thread);

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::threadEntry_sleep(Thread *thread, std::unique_lock<std::mutex> &mtx)
    {
        assert(Thread::current() == thread);

#ifndef NDEBUG
        {
            std::lock_guard<std::mutex> l(_threadsMtx);
            assert(_threads.end() != _threads.find(std::this_thread::get_id()));
        }
#endif

        _threadsCv.wait(mtx);
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Scheduler::threadEntry_deinit(Thread *thread)
    {
        assert(Thread::current() == thread);

        std::lock_guard<std::mutex> l(_threadsMtx);

        TMThreads::iterator iter = _threads.find(std::this_thread::get_id());

        if(_threads.end() == iter)
        {
            return false;
        }

        _threads.erase(iter);

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
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

    ////////////////////////////////////////////////////////////////////////////////
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

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_deactivateAndStayEmpty(Coro *coro)
    {
        Thread *thread = Thread::current();

        thread->storeEmptyCoro(coro);

        Context *next = thread->isReleaseRequested() ? thread->context() : _coroListReady.dequeue();
        if(!next)
        {
            next = thread->context();
        }

        coro->switchTo(next);

        enqueuePerThreadCoros(thread);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_deactivateAndStayReady(Coro *coro)
    {
        Thread *thread = Thread::current();

        thread->storeReadyCoro(coro);

        Context *next = thread->isReleaseRequested() ? thread->context() : _coroListReady.dequeue();
        if(!next)
        {
            next = thread->context();
        }

        coro->switchTo(next);

        enqueuePerThreadCoros(thread);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::enqueuePerThreadCoros(Thread *thread)
    {
        if(Coro *empty = thread->fetchEmptyCoro())
        {
            _coroListEmpty.enqueue(empty);
        }
        if(Coro *ready = thread->fetchReadyCoro())
        {
            _coroListReady.enqueue(ready);
        }
    }
}}}
