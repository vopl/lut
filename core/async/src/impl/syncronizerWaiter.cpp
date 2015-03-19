#include "syncronizerWaiter.hpp"
#include "syncronizer.hpp"
#include "scheduler.hpp"

namespace dci { namespace async { namespace impl
{
    SyncronizerWaiter::SyncronizerWaiter(SyncronizerPtr *syncronizers, std::size_t amount)
        : _syncronizers(syncronizers)
        , _amount(amount)
        , _coro()
        , _waitAll()
        , _lastReleasedIdx(amount)
    {

    }

    SyncronizerWaiter::~SyncronizerWaiter()
    {

    }

    std::size_t SyncronizerWaiter::any()
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

        _waitAll = false;
        _coro = Scheduler::instance().currentCoro();
        assert(_coro);
        Scheduler::instance().hold();

        assert(_lastReleasedIdx < _amount);

        return _lastReleasedIdx;
    }

    void SyncronizerWaiter::all()
    {
        bool acquiredAll = true;

        for(std::size_t idx(0); idx<_amount; ++idx)
        {
            acquiredAll &= _syncronizers[idx]->registerWaiter(this, idx);
        }

        if(acquiredAll)
        {
            for(std::size_t idx(0); idx<_amount; ++idx)
            {
                _syncronizers[idx]->unregisterWaiterAndCommitAcquire(this, idx);
            }
        }
        else
        {
            _waitAll = true;
            _coro = Scheduler::instance().currentCoro();
            assert(_coro);
            Scheduler::instance().hold();
        }

    }

    bool SyncronizerWaiter::released(std::size_t releasedIdx)
    {
        assert(releasedIdx < _amount);


        if(_waitAll)
        {
            for(std::size_t idx(0); idx<_amount; ++idx)
            {
                if(_syncronizers[idx]->locked())
                {
                    return false;
                }
            }

            for(std::size_t idx(0); idx<_amount; ++idx)
            {
                _syncronizers[idx]->unregisterWaiterAndCommitAcquire(this, idx);
            }

            Scheduler::instance().ready(_coro);
            return true;
        }


        //any
        for(std::size_t idx(0); idx<_amount; ++idx)
        {
            if(idx == releasedIdx)
            {
                _syncronizers[_lastReleasedIdx]->unregisterWaiterAndCommitAcquire(this, releasedIdx);
            }
            else
            {
                _syncronizers[idx]->unregisterWaiter(this, idx);
            }
        }

        _lastReleasedIdx = releasedIdx;
        Scheduler::instance().ready(_coro);
        return true;
    }


}}}
