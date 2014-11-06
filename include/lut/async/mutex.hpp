#pragma once

#include "lut/async/sizeofImpl.hpp"
#include "lut/hiddenImpl.hpp"

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
        void lock();
        bool tryLock();

    public:
        void unlock();

    public:
        bool locked() const;

    public:
        using HiddenImpl<impl::Mutex>::pimpl;
        using HiddenImpl<impl::Mutex>::impl;
    };

}}
