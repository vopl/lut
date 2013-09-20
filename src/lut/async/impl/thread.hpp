#ifndef _LUT_ASYNC_IMPL_THREAD_HPP_
#define _LUT_ASYNC_IMPL_THREAD_HPP_

#include "lut/async/scheduler.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/context.hpp"

namespace lut { namespace async { namespace impl
{
    class Thread
    {
    public:
        Thread(Scheduler &scheduler, ThreadState *stateEvt);
        ~Thread();

        template <class StoppingMeter>
        ThreadUtilizationResult utilize(StoppingMeter &stoppingMeter);

        static Thread *instance();
    public:
        void releaseRequest();

    private:
        Scheduler &_scheduler;
        ThreadState *_stateEvt;

        std::mutex _mtx;
        std::condition_variable _cv;

        bool _releaseRequest;

        static thread_local Thread *_thread;
        Context *_context;
    };

    template <class StoppingMeter>
    ThreadUtilizationResult Thread::utilize(StoppingMeter &stoppingMeter)
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

            while(!_releaseRequest && !stoppingMeter.signalled())
            {
                lock.unlock();
                bool utilizeResult = _scheduler.threadEntry_utilize(this);
                lock.lock();

                if(!utilizeResult)
                {
                    stoppingMeter.wait(_cv, lock);
                }
            }

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

}}}

#endif
