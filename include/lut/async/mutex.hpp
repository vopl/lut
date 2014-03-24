#ifndef _LUT_ASYNC_MUTEX_HPP_
#define _LUT_ASYNC_MUTEX_HPP_

#include "lut/async/hiddenImpl.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Mutex;
    }

    class Mutex
        : private HiddenImpl<impl::Mutex>
    {
        Mutex(const Mutex &from) = delete;
        Mutex &operator=(const Mutex &from) = delete;

    public:
        Mutex();
        ~Mutex();

    public:
        void acquire();
        bool tryAcquire();

    public:
        void release();

    public:
        bool isAcquired() const;

    public:
        using HiddenImpl<impl::Mutex>::pimpl;
        using HiddenImpl<impl::Mutex>::impl;
    };

}}

#endif
