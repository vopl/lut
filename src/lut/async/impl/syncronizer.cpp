#include "lut/async/stable.hpp"
#include "lut/async/impl/syncronizer.hpp"

#include <algorithm>

namespace lut { namespace async { namespace impl
{

    Syncronizer::Syncronizer()
        : _acqiured()
        , _waiters()
    {
    }

    Syncronizer::~Syncronizer()
    {
    }

    bool Syncronizer::registerWaiter(AcquireWaiter *acquireWaiter, std::size_t waiterData)
    {
        _waiters.emplace_back(acquireWaiter, waiterData);
        return !_acqiured;
    }

    void Syncronizer::unregisterWaiterAndCommitAcquire(AcquireWaiter *acquireWaiter, std::size_t waiterData)
    {
        unregisterWaiter(acquireWaiter, waiterData);

        assert(!_acqiured);
        _acqiured = true;
    }

    void Syncronizer::unregisterWaiter(AcquireWaiter *acquireWaiter, std::size_t waiterData)
    {
        AcquireWaiterWithData acquireWaiterWithData {acquireWaiter, waiterData};
        TVWaiters::reverse_iterator iter = std::find(_waiters.rbegin(), _waiters.rend(), acquireWaiterWithData);
        assert(_waiters.rend() != iter);
        _waiters.erase((iter+1).base());
    }

    Syncronizer::AcquireWaiterWithData::AcquireWaiterWithData(AcquireWaiter *acquireWaiter, std::size_t waiterData)
        : _acquireWaiter(acquireWaiter)
        , _waiterData(waiterData)
    {
    }

    bool Syncronizer::AcquireWaiterWithData::operator==(const AcquireWaiterWithData &with) const
    {
        return _acquireWaiter == with._acquireWaiter &&
                _waiterData == with._waiterData;
    }

}}}
