#ifndef _LUT_ASYNC_IMPL_MUTEX_HPP_
#define _LUT_ASYNC_IMPL_MUTEX_HPP_

#include "lut/async/impl/syncronizer.hpp"
#include <cstdint>

namespace lut { namespace async { namespace impl
{

    class Mutex
        : public Syncronizer
    {
    public:
        Mutex();
        virtual ~Mutex();

    public:
        virtual bool locked() const override;
        virtual void lock() override;
        bool tryLock();
        void unlock();
    };

}}}

#endif
