#pragma once

#include "lut/async/sizeProvider.hpp"
#include "lut/hiddenImpl/single.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Mutex;
    }

    class Mutex
        : public hiddenImpl::Single<impl::Mutex>
    {
        Mutex(const Mutex &from) = delete;
        Mutex &operator=(const Mutex &from) = delete;

    public:
        Mutex();
        ~Mutex();

    public:
        void lock();
        bool tryLock();

    public:
        void unlock();

    public:
        bool locked() const;

    public:
        using Single<impl::Mutex>::pimpl;
        using Single<impl::Mutex>::impl;
    };

}}
