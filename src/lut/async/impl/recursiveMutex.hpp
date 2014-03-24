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
        ~RecursiveMutex();

    public:
        void acquire();
        bool tryAcquire();

    public:
        void release();

    public:
        bool isAcquired() const;

    private:
        ctx::Coro * _owner;
        std::size_t _ownerDepth;
    };

}}}

#endif
