#include "lut/async/stable.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    thread_local Thread *Thread::_current(nullptr);

    ////////////////////////////////////////////////////////////////////////////////
    Thread::Thread(Scheduler *scheduler, ThreadState *stateEvt)
        : _id(std::this_thread::get_id())
        , _scheduler(scheduler)
        , _stateEvt(stateEvt)
        , _releaseRequest(false)
        , _rootContext()
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
        assert(!_rootContext);
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
            assert(!_rootContext);
            ctx::Root rootContext;
            _rootContext = &rootContext;


            if(_stateEvt)
            {
                _stateEvt->set(ThreadStateValue::inWork);
            }

            _scheduler->threadEntry_utilize(this);

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
    std::thread::id Thread::getId()
    {
        return _id;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Thread *Thread::getCurrent()
    {
        return _current;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ctx::Root *Thread::getRootContext()
    {
        return _rootContext;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scheduler *Thread::getScheduler()
    {
        return _scheduler;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Thread::storeEmptyCoro(ctx::Coro *coro)
    {
        assert(!_storedEmptyCoro);
        _storedEmptyCoro = coro;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Thread::storeReadyCoro(ctx::Coro *coro)
    {
        assert(!_storedReadyCoro);
        _storedReadyCoro = coro;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ctx::Coro *Thread::fetchEmptyCoro()
    {
        if(_storedEmptyCoro)
        {
            ctx::Coro *coro = _storedEmptyCoro;
            _storedEmptyCoro = nullptr;
            return coro;
        }

        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ctx::Coro *Thread::fetchReadyCoro()
    {
        if(_storedReadyCoro)
        {
            ctx::Coro *coro = _storedReadyCoro;
            _storedReadyCoro = nullptr;
            return coro;
        }

        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Thread::setCurrentCoro(ctx::Coro *coro)
    {
        _currentCoro = coro;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ctx::Coro *Thread::getCurrentCoro()
    {
        return _currentCoro;
    }

}}}
