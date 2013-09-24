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
    void Scheduler::threadEntry_utilize(Thread *thread)
    {
        assert(Thread::current() == thread);

        Coro *coro = _coroListReady.dequeue();
        while(!coro)
        {
            coro = _coroListReady.dequeue();
        }

        assert(Thread::current() == thread);

        assert(!thread->getCurrentCoro());

        thread->setCurrentCoro(coro);
        thread->context()->switchTo(coro);

        assert(Thread::current() == thread);
        enqueuePerThreadCoros(thread);
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
            coro = Coro::make();
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
            coro = Coro::make();
        }

        coro->setCode(std::forward<Task>(code));
        _coroListReady.enqueue(coro);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::yield()
    {
        Thread *thread = Thread::current();
        Coro *coro = thread->getCurrentCoro();
        assert(coro);
        if(coro)
        {
            coroEntry_stayReadyAndDeactivate(coro);
        }
    }


    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_stayEmptyAndDeactivate(Coro *coro)
    {
        {
            Thread *thread = Thread::current();
            thread->storeEmptyCoro(coro);

            for(;;)
            {
                if(thread->isReleaseRequested())
                {
                    thread->setCurrentCoro(nullptr);
                    coro->switchTo(thread->context());
                    break;
                }
                else
                {
                    Coro *next = _coroListReady.dequeue();
                    if(next)
                    {
                        thread->setCurrentCoro(next);
                        coro->switchTo(next);
                        break;
                    }
                }
            }
        }

        {
            Thread *thread = Thread::current();
            thread->scheduler()->enqueuePerThreadCoros(thread);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_stayReadyAndDeactivate(Coro *coro)
    {
        Thread *thread = Thread::current();
        if(thread->isReleaseRequested())
        {
            thread->storeReadyCoro(coro);
            thread->setCurrentCoro(nullptr);
            coro->switchTo(thread->context());

            Thread *thread = Thread::current();
            thread->scheduler()->enqueuePerThreadCoros(thread);
            return;
        }

        Coro *next = _coroListReady.dequeue();
        if(next)
        {
            thread->storeReadyCoro(coro);
            thread->setCurrentCoro(next);
            coro->switchTo(next);

            Thread *thread = Thread::current();
            thread->scheduler()->enqueuePerThreadCoros(thread);
            return;
        }
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
