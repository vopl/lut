#pragma once

#include "lut/async/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Mutex;
    }

    class Mutex
        : public hiddenImpl::Layout<impl::Mutex>
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
        using hiddenImpl::Layout<impl::Mutex>::pimpl;
        using hiddenImpl::Layout<impl::Mutex>::impl;
    };

}}
