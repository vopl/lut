#include "lut/async/stable.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    thread_local Thread *Thread::_current(nullptr);

    ////////////////////////////////////////////////////////////////////////////////
    Thread::Thread(Scheduler *scheduler, ThreadState *stateEvt)
        : _scheduler(scheduler)
        , _stateEvt(stateEvt)
        , _releaseRequest(false)
        , _context()
        , _storedEmptyCoro()
        , _storedReadyCoro()
        , _currentCoro()
    {
        assert(!_current);
    }

    ////////////////////////////////////////////////////////////////////////////////
    Thread::~Thread()
    {
        assert(!_current);
        assert(!_context);
        assert(!_storedEmptyCoro);
        assert(!_storedReadyCoro);
        assert(!_currentCoro);
    }

    ////////////////////////////////////////////////////////////////////////////////
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
            assert(!_context);
            Context context;
            _context = &context;


            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::inWork);
            }

            _scheduler->threadEntry_utilize(this);

            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::doneWork);
            }

            _context = nullptr;
        }

        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::deinit);
        }

        _scheduler->threadEntry_deinit(this);
        _current = nullptr;

        if(_stateEvt)
        {
            _stateEvt->set(ThreadStateValue::deinited);
        }

        return ThreadUtilizationResult::releaseRequest;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Thread::releaseRequest()
    {
        _releaseRequest.store(true);
    }

    bool Thread::isReleaseRequested()
    {
        return _releaseRequest.load();
    }

    ////////////////////////////////////////////////////////////////////////////////
    Thread *Thread::current()
    {
        return _current;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Context *Thread::context()
    {
        return _context;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scheduler *Thread::scheduler()
    {
        return _scheduler;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Thread::storeEmptyCoro(Coro *coro)
    {
        assert(!_storedEmptyCoro);
        _storedEmptyCoro = coro;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Thread::storeReadyCoro(Coro *coro)
    {
        assert(!_storedReadyCoro);
        _storedReadyCoro = coro;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Coro *Thread::fetchEmptyCoro()
    {
        if(_storedEmptyCoro)
        {
            Coro *coro = _storedEmptyCoro;
            _storedEmptyCoro = nullptr;
            return coro;
        }

        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Coro *Thread::fetchReadyCoro()
    {
        if(_storedReadyCoro)
        {
            Coro *coro = _storedReadyCoro;
            _storedReadyCoro = nullptr;
            return coro;
        }

        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Thread::setCurrentCoro(Coro *coro)
    {
        _currentCoro = coro;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Coro *Thread::getCurrentCoro()
    {
        return _currentCoro;
    }

}}}
