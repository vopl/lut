#pragma once

#include "syncronizerWaiter.hpp"
#include <vector>

namespace dci { namespace async { namespace impl
{

    class Syncronizer
    {
    public:
        Syncronizer();
        ~Syncronizer();

    public:
        bool registerWaiter(SyncronizerWaiter *waiter, std::size_t data);
        void unregisterWaiterAndCommitAcquire(SyncronizerWaiter *waiter, std::size_t data);
        void unregisterWaiter(SyncronizerWaiter *waiter, std::size_t data);

        virtual bool locked() const = 0;
        virtual void lock() = 0;
        void unlock();

    private:
        struct WaiterWithData
        {
            SyncronizerWaiter *_waiter;
            std::size_t _data;

            WaiterWithData(SyncronizerWaiter *waiter, std::size_t data);
            bool operator==(const WaiterWithData &with) const;
        };

        using TVWaiters = std::vector<WaiterWithData>;
        TVWaiters _waiters;
    };

    using SyncronizerPtr = Syncronizer *;

}}}
