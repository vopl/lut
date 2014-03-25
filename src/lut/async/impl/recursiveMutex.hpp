#ifndef _LUT_ASYNC_IMPL_RECURSIVEMUTEX_HPP_
#define _LUT_ASYNC_IMPL_RECURSIVEMUTEX_HPP_

#include "lut/async/impl/syncronizer.hpp"
#include "lut/async/impl/ctx/coro.hpp"
#include <cstdint>

namespace lut { namespace async { namespace impl
{

    class RecursiveMutex
        : public Syncronizer
    {
    public:
        RecursiveMutex();
        virtual ~RecursiveMutex();

    public:
        virtual bool locked() const override;
        virtual void lock() override;
        bool tryLock();
        void unlock();

    private:
        ctx::Coro * _owner;
        std::size_t _ownerDepth;
    };

}}}

#endif
