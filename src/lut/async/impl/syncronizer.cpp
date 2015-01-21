#include "lut/stable.hpp"
#include "lut/async/impl/syncronizer.hpp"

#include <algorithm>

namespace lut { namespace async { namespace impl
{

    Syncronizer::Syncronizer()
        : _waiters()
    {
    }

    Syncronizer::~Syncronizer()
    {
    }

    bool Syncronizer::registerWaiter(SyncronizerWaiter *waiter, std::size_t data)
    {
        _waiters.emplace_back(waiter, data);
        return !locked();
    }

    void Syncronizer::unregisterWaiterAndCommitAcquire(SyncronizerWaiter *waiter, std::size_t data)
    {
        unregisterWaiter(waiter, data);

        assert(!locked());
        lock();
    }

    void Syncronizer::unregisterWaiter(SyncronizerWaiter *waiter, std::size_t data)
    {
        WaiterWithData syncronizerWaiterWithData {waiter, data};
        TVWaiters::reverse_iterator iter = std::find(_waiters.rbegin(), _waiters.rend(), syncronizerWaiterWithData);
        assert(_waiters.rend() != iter);
        _waiters.erase((iter+1).base());
    }

    void Syncronizer::unlock()
    {
        if(!_waiters.empty())
        {
            for(WaiterWithData &syncronizerWaiterWithData : _waiters)
            {
                if(syncronizerWaiterWithData._waiter->released(syncronizerWaiterWithData._data))
                {
                    return;
                }
            }
        }
    }

    Syncronizer::WaiterWithData::WaiterWithData(SyncronizerWaiter *waiter, std::size_t data)
        : _waiter(waiter)
        , _data(data)
    {
    }

    bool Syncronizer::WaiterWithData::operator==(const WaiterWithData &with) const
    {
        return _waiter == with._waiter &&
                _data == with._data;
    }

}}}
