#include "lut/async/stable.hpp"
#include "lut/async/impl/acquireWaiter.hpp"
#include "lut/async/impl/syncronizer.hpp"
#include "lut/async/impl/scheduler.hpp"

namespace lut { namespace async { namespace impl
{
    AcquireWaiter::AcquireWaiter(SyncronizerPtr *syncronizers, std::size_t amount)
        : _syncronizers(syncronizers)
        , _amount(amount)
        , _lastNotifiedIdx(amount)
    {

    }

    AcquireWaiter::~AcquireWaiter()
    {

    }

    std::size_t AcquireWaiter::any()
    {
        for(std::size_t idx(0); idx<_amount; ++idx)
        {
            if(_syncronizers[idx]->registerWaiter(this, idx))
            {
                _syncronizers[idx]->unregisterWaiterAndCommitAcquire(this, idx);

                for(std::size_t idx2(0); idx2<idx; ++idx2)
                {
                    _syncronizers[idx]->unregisterWaiter(this, idx);
                }

                return idx;
            }
        }

        Scheduler::instance().hold();

        assert(_lastNotifiedIdx < _amount);

        for(std::size_t idx(0); idx<_amount; ++idx)
        {
            if(idx == _lastNotifiedIdx)
            {
                _syncronizers[_lastNotifiedIdx]->unregisterWaiterAndCommitAcquire(this, _lastNotifiedIdx);
            }
            else
            {
                _syncronizers[idx]->unregisterWaiter(this, idx);
            }
        }

        return _lastNotifiedIdx;
    }

    void AcquireWaiter::all()
    {
        bool acquiredAll = true;

        for(std::size_t idx(0); idx<_amount; ++idx)
        {
            acquiredAll &= _syncronizers[idx]->registerWaiter(this, idx);
        }

        if(!acquiredAll)
        {
            Scheduler::instance().hold();
        }

        for(std::size_t idx(0); idx<_amount; ++idx)
        {
            _syncronizers[idx]->unregisterWaiterAndCommitAcquire(this, idx);
        }
    }

}}}
