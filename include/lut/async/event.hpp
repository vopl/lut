#pragma once

#include "lut/async/sizeofImpl.hpp"
#include "lut/hiddenImpl.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Event;
    }

    class Event
        : private HiddenImpl<impl::Event>
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
        using HiddenImpl<impl::Event>::pimpl;
        using HiddenImpl<impl::Event>::impl;
    };

}}
