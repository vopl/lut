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

//        while(Coro *ready = _coroListReady.dequeue())
//        {
//            delete ready;
//        }

        assert(_threads.empty());
    }

    ////////////////////////////////////////////////////////////////////////////////
    ThreadReleaseResult Scheduler::threadRelease(const std::thread::id &id)
    {
        std::lock_guard<std::mutex> l(_threadsMtx);
        TVThreads::iterator iter = _threads.begin();
        TVThreads::iterator end = _threads.end();

        for(; iter!=end; ++iter)
        {
            Thread *thread = *iter;
            if(thread->getId() == id)
            {
                thread->releaseRequest();
                return ThreadReleaseResult::ok;
            }
        }

        return ThreadReleaseResult::notInWork;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Scheduler::threadEntry_init(Thread *thread)
    {
        assert(Thread::getCurrent() == thread);

        std::lock_guard<std::mutex> l(_threadsMtx);

        _threads.push_back(thread);

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::threadEntry_utilize(Thread *thread)
    {
        assert(Thread::getCurrent() == thread);

        Coro *coro = fetchReadyCoro4Thread(thread);
        assert(coro || thread->isReleaseRequested());

        assert(Thread::getCurrent() == thread);

        assert(!thread->getCurrentCoro());

        if(coro)
        {
            thread->setCurrentCoro(coro);
            thread->getRootContext()->switchTo(coro);

            assert(Thread::getCurrent() == thread);
        }
        else
        {
            assert(thread->isReleaseRequested());
            //nothing, done work
        }

        enqueuePerThreadCoros(thread, true);
        _coroListReady.gripFrom(thread->_coroListReady);
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Scheduler::threadEntry_deinit(Thread *thread)
    {
        assert(Thread::getCurrent() == thread);

        std::lock_guard<std::mutex> l(_threadsMtx);

        TVThreads::iterator iter = _threads.begin();
        TVThreads::iterator end = _threads.end();

        for(; iter!=end; ++iter)
        {
            if(*iter == thread)
            {
                _threads.erase(iter);
                return true;
            }
        }

        return false;
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

        Thread *thread = Thread::getCurrent();
        if(thread)
        {
            thread->_coroListReady.enqueue(coro);
        }
        else
        {
            _coroListReady.enqueue(coro);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::yield()
    {
        Thread *thread = Thread::getCurrent();
        Coro *coro = thread->getCurrentCoro();
        assert(coro);
        if(coro)
        {
            coroEntry_stayReadyAndDeactivate(coro, thread);
        }
    }


    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_stayEmptyAndDeactivate(Coro *coro, Thread *thread)
    {
        assert(Thread::getCurrent() == thread);
        assert(thread->getCurrentCoro() == coro);

        thread->storeEmptyCoro(coro);

        Coro *next = fetchReadyCoro4Thread(thread);

        if(next)
        {
            thread->setCurrentCoro(next);
            coro->switchTo(next);
        }
        else
        {
            assert(thread->isReleaseRequested());
            thread->setCurrentCoro(nullptr);
            coro->switchTo(thread->getRootContext());
        }

        {
            Thread *thread = Thread::getCurrent();
            thread->getScheduler()->enqueuePerThreadCoros(thread);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::coroEntry_stayReadyAndDeactivate(Coro *coro, Thread *thread)
    {
        assert(Thread::getCurrent() == thread);
        assert(thread->getCurrentCoro() == coro);

        if(thread->isReleaseRequested())
        {
            thread->storeReadyCoro(coro);
            thread->setCurrentCoro(nullptr);

            coro->switchTo(thread->getRootContext());

            {
                Thread *thread = Thread::getCurrent();
                thread->getScheduler()->enqueuePerThreadCoros(thread);
            }

            return;
        }

        Coro *next = thread->_coroListReady.dequeue();

        if(next)
        {
            thread->storeReadyCoro(coro);
            thread->setCurrentCoro(next);

            coro->switchTo(next);

            {
                Thread *thread = Thread::getCurrent();
                thread->getScheduler()->enqueuePerThreadCoros(thread);
            }
        }
        else
        {
            //continue current coro
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::enqueuePerThreadCoros(Thread *thread, bool threadWantExit)
    {
        if(Coro *empty = thread->fetchEmptyCoro())
        {
            _coroListEmpty.enqueue(empty);
        }
        if(Coro *ready = thread->fetchReadyCoro())
        {
            if(threadWantExit)
            {
                _coroListReady.enqueue(ready);
            }
            else
            {
                thread->_coroListReady.enqueue(ready);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    Coro *Scheduler::fetchReadyCoro4Thread(Thread *thread)
    {
        assert(Thread::getCurrent() == thread);

        for(;;)
        {
            if(thread->isReleaseRequested())
            {
                return nullptr;
            }

            Coro *coro = thread->_coroListReady.dequeue();
            if(coro)
            {
                return coro;
            }

            if(_coroListReady.sizeRelaxed())
            {
                thread->_coroListReady.gripFrom(_coroListReady);
                coro = thread->_coroListReady.dequeue();
                if(coro)
                {
                    return coro;
                }
            }

            //lock relocator

            if(relocateReadyCoros(thread))
            {
                //unlock relocator
                Coro *coro = thread->_coroListReady.dequeue();
                if(coro)
                {
                    return coro;
                }
            }
            else
            {
                if(thread->isReleaseRequested())
                {
                    //unlock relocator
                    return nullptr;
                }

                //unlock relocator and sleep
            }
        }

        assert(!"never here");
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Scheduler::relocateReadyCoros(Thread *thread)
    {
        assert(Thread::getCurrent() == thread);
        assert(thread->_coroListReady.emptyRelaxed());

        using ThreadWithSize = std::pair<Thread *, size_t>;

        assert(!"not impl");
    }

}}}
