#pragma once

#include "lut/async/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Event;
    }

    class Event
        : public hiddenImpl::Layout<impl::Event>
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
        using hiddenImpl::Layout<impl::Event>::pimpl;
        using hiddenImpl::Layout<impl::Event>::impl;
    };

}}
