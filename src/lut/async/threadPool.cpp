#include "async/stable.hpp"
#include "async/threadPool.hpp"
#include <cassert>

namespace async
{
    ThreadPool::ThreadPool(const async::ThreadUtilizer &tu, size_t amount)
        : _tu(tu)
        , _threads(amount)
        , _states(amount)
    {
        for(size_t idx(0); idx<amount; idx++)
        {
            std::thread &thread = _threads[idx];
            ThreadState *stateEvt = &_states[idx];
            thread = std::thread([this,stateEvt]{
                EThreadUtilizationResult etur = _tu.te_utilize(stateEvt);
                (void)etur;
                assert(etur_releaseRequest == etur);
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

            _states[idx].waitNot(ThreadState::init);

            EThreadReleaseResult etrr = _tu.release(thread.native_handle());
            (void)etrr;
            assert(etrr_ok == etrr);
        }
        for(size_t idx(0); idx<amount; idx++)
        {
            std::thread &thread = _threads[idx];
            thread.join();
        }
        _threads.clear();
        _states.clear();
    }
}
