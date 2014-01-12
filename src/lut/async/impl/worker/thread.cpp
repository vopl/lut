#include "lut/async/stable.hpp"
#include "lut/async/impl/worker/thread.hpp"
#include "lut/async/impl/scheduler.hpp"


namespace lut { namespace async { namespace impl { namespace worker
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    __thread Thread *Thread::_current = nullptr;

    Thread::Thread(Scheduler *scheduler, ThreadState *stateEvt)
        : _id(std::this_thread::get_id())
        , _scheduler(scheduler)
        , _stateEvt(stateEvt)
        , _releaseRequest()
        , _rootContext()
    {
    }

    Thread::~Thread()
    {
    }

    ThreadUtilizationResult Thread::utilize()
    {
        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::init);
        }

        assert(!_current);
        _current = this;

        if(!_scheduler->threadEntry_init(this))
        {
            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::deinited);
            }

            return ThreadUtilizationResult::notBeenUtilized;
        }

        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::inited);
        }

        //work
        {
            assert(!_rootContext);
            ctx::Root rootContext;
            _rootContext = &rootContext;

            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::inWork);
            }

            workLoop();

            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::doneWork);
            }

            _rootContext = nullptr;
        }

        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::deinit);
        }

        bool b = _scheduler->threadEntry_deinit(this);
        assert(b);
        (void)b;

        assert(this == _current);
        _current = nullptr;

        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::deinited);
        }

        return ThreadUtilizationResult::releaseRequest;
    }

    Thread *Thread::getCurrent()
    {
        return _current;
    }

    void Thread::wakeUp()
    {
        std::unique_lock<std::mutex> l(_mtx);
        _cv.notify_one();
    }

    void Thread::yield()
    {
        assert(0);
    }

    void Thread::workLoop()
    {
        std::unique_lock<std::mutex> l(_mtx);

        while(!_releaseRequest.load())
        {
            ctx::Coro *coro = dequeueReadyCoro();
            if(coro)
            {
                _rootContext->switchTo(coro);
                continue;
            }

            if(!_scheduler->threadEntry_provideWork(this))
            {
                _cv.wait(l);
                _scheduler->threadEntry_workContinued(this);
            }
        }
    }

}}}}
