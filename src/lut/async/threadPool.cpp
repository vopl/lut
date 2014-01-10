#include "lut/async/stable.hpp"
#include "lut/async/stable.hpp"
#include "lut/async/threadPool.hpp"

namespace lut { namespace async
{
    ThreadPool::ThreadPool(Scheduler &scheduler, size_t amount)
        : _scheduler(scheduler)
        , _threads(amount)
        , _states(amount)
    {
        for(size_t idx(0); idx<amount; idx++)
        {
            std::thread &thread = _threads[idx];
            ThreadState *stateEvt = &_states[idx];
            thread = std::thread([this,stateEvt]{
                ThreadUtilizationResult etur = _scheduler.utilizeThisThread(stateEvt);
                (void)etur;
                assert(ThreadUtilizationResult::releaseRequest == etur);
            });
        }
    }

    ThreadPool::~ThreadPool()
    {
        size_t amount = _states.size();
        for(size_t idx(0); idx<amount; idx++)
        {
            std::thread &thread = _threads[idx];
            assert(thread.get_id() != std::this_thread::get_id());

            _states[idx].waitNot(ThreadStateValue::init);

            ThreadReleaseResult etrr = _scheduler.releaseThread(thread.native_handle());
            (void)etrr;
            assert(ThreadReleaseResult::ok == etrr);
        }
        for(size_t idx(0); idx<amount; idx++)
        {
            std::thread &thread = _threads[idx];
            thread.join();
        }
        _threads.clear();
        _states.clear();
    }
}}
