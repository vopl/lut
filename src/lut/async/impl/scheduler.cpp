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

        {
            std::lock_guard<std::mutex> l(_threadsCvMtx);
            iter->second->releaseRequest();
            _threadsCv.notify_all();
        }

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

        Coro *coro = _coroListReady.dequeue();
        if(!coro)
        {
            return false;
        }

        assert(Thread::current() == thread);

        assert(!thread->getCurrentCoro());
        thread->setCurrentCoro(coro);
        thread->context()->switchTo(coro);

        assert(Thread::current() == thread);
        enqueuePerThreadCoros(thread);

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Scheduler::threadEntry_sleep(Thread *thread)
    {
        assert(Thread::current() == thread);

        if(thread->isReleaseRequested())
        {
            return false;
        }

        std::unique_lock<std::mutex> lock(_threadsCvMtx);
        _threadsCv.wait(lock);

        return !thread->isReleaseRequested();
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

        std::unique_lock<std::mutex> lock(_threadsCvMtx);
        _threadsCv.notify_all();
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

        std::unique_lock<std::mutex> lock(_threadsCvMtx);
        _threadsCv.notify_all();
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::yield()
    {
        Thread *thread = Thread::current();
        Coro *coro = thread->getCurrentCoro();
        if(coro)
        {
            coroEntry_deactivateAndStayReady(coro);
        }
    }


    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_deactivateAndStayEmpty(Coro *coro)
    {
        Thread *thread = Thread::current();

        thread->storeEmptyCoro(coro);

        if(thread->isReleaseRequested())
        {
            thread->setCurrentCoro(nullptr);
            coro->switchTo(thread->context());
        }
        else
        {
            Coro *next = _coroListReady.dequeue();
            if(next)
            {
                thread->setCurrentCoro(next);
                coro->switchTo(next);
            }
            else
            {
                thread->setCurrentCoro(nullptr);
                coro->switchTo(thread->context());
            }
        }

        thread = Thread::current();
        enqueuePerThreadCoros(thread);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_deactivateAndStayReady(Coro *coro)
    {
        Thread *thread = Thread::current();

        thread->storeReadyCoro(coro);

        if(thread->isReleaseRequested())
        {
            thread->setCurrentCoro(nullptr);
            coro->switchTo(thread->context());
        }
        else
        {
            Coro *next = _coroListReady.dequeue();
            if(next)
            {
                thread->setCurrentCoro(next);
                coro->switchTo(next);
            }
            else
            {
                thread->setCurrentCoro(nullptr);
                coro->switchTo(thread->context());
            }
        }

        thread = Thread::current();
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
