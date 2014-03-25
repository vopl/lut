#ifndef _LUT_ASYNC_IMPL_SYNCRONIZER_HPP_
#define _LUT_ASYNC_IMPL_SYNCRONIZER_HPP_

#include "lut/async/impl/acquireWaiter.hpp"
#include <vector>

namespace lut { namespace async { namespace impl
{

    class Syncronizer
    {
    public:
        Syncronizer();
        ~Syncronizer();

    public:
        bool registerWaiter(AcquireWaiter *acquireWaiter, std::size_t waiterData);
        void unregisterWaiterAndCommitAcquire(AcquireWaiter *acquireWaiter, std::size_t waiterData);
        void unregisterWaiter(AcquireWaiter *acquireWaiter, std::size_t waiterData);

    protected:
        bool _acqiured;

        struct AcquireWaiterWithData
        {
            AcquireWaiter *_acquireWaiter;
            std::size_t _waiterData;

            AcquireWaiterWithData(AcquireWaiter *acquireWaiter, std::size_t waiterData);
            bool operator==(const AcquireWaiterWithData &with) const;
        };

        using TVWaiters = std::vector<AcquireWaiterWithData>;
        TVWaiters _waiters;
    };

    using SyncronizerPtr = Syncronizer *;

}}}

#endif
