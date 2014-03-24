#ifndef _LUT_ASYNC_RECURSIVEMUTEX_HPP_
#define _LUT_ASYNC_RECURSIVEMUTEX_HPP_

#include "lut/async/hiddenImpl.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class RecursiveMutex;
    }

    class RecursiveMutex
        : private HiddenImpl<impl::RecursiveMutex>
    {
        RecursiveMutex(const RecursiveMutex &from) = delete;
        RecursiveMutex &operator=(const RecursiveMutex &from) = delete;

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

    public:
        using HiddenImpl<impl::RecursiveMutex>::pimpl;
        using HiddenImpl<impl::RecursiveMutex>::impl;
    };

}}

#endif
