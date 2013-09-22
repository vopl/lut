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

    ThreadUtilizationResult Thread::utilize()
    {
        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::init);
        }

        assert(!_thread);
        _thread = this;
        if(!_scheduler.threadEntry_init(this))
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

        std::unique_lock<std::mutex> lock(_mtx);

        //work
        {
            assert(!_context);
            Context context;
            _context = &context;


            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::inWork);
            }

            while(!_releaseRequest)
            {
                lock.unlock();
                bool utilizeResult = _scheduler.threadEntry_utilize(this);
                lock.lock();

                if(!utilizeResult)
                {
                    _cv.wait(lock);
                }
            }

            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::doneWork);
            }

            _context = nullptr;
        }

        lock.unlock();

        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::deinit);
        }

        _scheduler.threadEntry_deinit(this);
        _thread = nullptr;

        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::deinited);
        }

        return _releaseRequest ?
                    ThreadUtilizationResult::releaseRequest :
                    ThreadUtilizationResult::limitExhausted;
    }

    Thread *Thread::instance()
    {
        return _thread;
    }

    Context *Thread::context()
    {
        return _context;
    }

    void Thread::releaseRequest()
    {
        std::lock_guard<std::mutex> l(_mtx);

        _releaseRequest = true;
        _cv.notify_all();
    }

}}}
