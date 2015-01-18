#pragma once

#include "lut/async/sizeProvider.hpp"
#include "lut/hiddenImpl/single.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Event;
    }

    class Event
        : public hiddenImpl::Single<impl::Event>
    {
        Event(const Event &from) = delete;
        Event &operator=(const Event &from) = delete;

    public:
        Event(bool autoReset = true);
        ~Event();

    public:
        void acquire();
        bool tryAcquire();

    public:
        void set();
        void reset();

    public:
        bool isSignalled() const;

    public:
        using Single<impl::Event>::pimpl;
        using Single<impl::Event>::impl;
    };

}}
