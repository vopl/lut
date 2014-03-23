#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/worker/thread.hpp"
#include "lut/async/impl/ctx/stackAllocator.hpp"

#include <algorithm>

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
        return ThreadReleaseResult::notInWork;
    }

    bool Scheduler::threadEntry_init(worker::Thread *thread)
    {
        assert(worker::Thread::getCurrent() == thread);

//        if(!ctx::StackAllocator::threadInit())
//        {
//            return false;
//        }

        std::lock_guard<std::mutex> l(_threadsMtx);

        _threads.push_back(thread);

        return true;
    }

    bool Scheduler::threadEntry_deinit(worker::Thread *thread)
    {
        assert(worker::Thread::getCurrent() == thread);

        std::lock_guard<std::mutex> l(_threadsMtx);

        TVThreads::iterator iter = _threads.begin();
        TVThreads::iterator end = _threads.end();

        for(; iter!=end; ++iter)
        {
            if(*iter == thread)
            {
                _threads.erase(iter);

//                ctx::StackAllocator::threadDeinit();
                return true;
            }
        }

        return false;
    }

    bool Scheduler::threadEntry_provideWork(worker::Thread *thread)
    {
        assert(worker::Thread::getCurrent() == thread);

        if(_effort.haveReadyCoros())
        {
            _effort.moveReadyCorosTo(thread);
            return true;
        }

        if(_effort.haveTasks())
        {
            _effort.moveTasksTo(thread);
            return true;
        }

        assert(!"from other threads");

        std::unique_lock<std::mutex> l(_threadsWaitMtx);
        _threadsWait.push_back(thread);

        return false;
    }

    void Scheduler::threadEntry_workContinued(worker::Thread *thread)
    {
        assert(worker::Thread::getCurrent() == thread);

        std::unique_lock<std::mutex> l(_threadsWaitMtx);
        _threadsWait.erase(std::find_if(_threadsWait.begin(), _threadsWait.end(), [thread](worker::Thread *v)->bool{return thread == v;}));
    }

    void Scheduler::spawn(Task *task)
    {
        worker::Thread *thread = worker::Thread::getCurrent();
        if(thread)
        {
            return thread->enqueueTask(task);
        }

        {
            std::unique_lock<std::mutex> l(_threadsWaitMtx);
            if(!_threadsWait.empty())
            {
                _threadsWait[0]->enqueueTask(task);
                _threadsWait[0]->wakeUp();
                return;
            }
        }

        _effort.enqueueTask(task);
    }

    void Scheduler::yield()
    {
        worker::Thread *thread = worker::Thread::getCurrent();
        if(!thread)
        {
            assert(0);
            return;
        }
        thread->yield();
    }

}}}
