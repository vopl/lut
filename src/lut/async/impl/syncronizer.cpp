#include "lut/async/stable.hpp"
#include "lut/async/impl/syncronizer.hpp"

namespace lut { namespace async { namespace impl
{

    Syncronizer::Syncronizer()
    {
        assert(0);
    }

    Syncronizer::~Syncronizer()
    {
        assert(0);
    }

    bool Syncronizer::registerWaiter(AcquireWaiter *acquireWaiter, std::size_t waiterData)
    {
        assert(0);
    }

    void Syncronizer::unregisterWaiterAndCommitAcquire(AcquireWaiter *acquireWaiter, std::size_t waiterData)
    {
        assert(0);
    }

    void Syncronizer::unregisterWaiter(AcquireWaiter *acquireWaiter, std::size_t waiterData)
    {
        assert(0);
    }

}}}
