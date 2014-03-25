#ifndef _LUT_ASYNC_IMPL_SYNCRONIZER_HPP_
#define _LUT_ASYNC_IMPL_SYNCRONIZER_HPP_

#include "lut/async/impl/acquireWaiter.hpp"

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
    };

    using SyncronizerPtr = Syncronizer *;

}}}

#endif
