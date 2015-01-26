#pragma once

#include "lut/async/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Event;
    }

    class Event
        : public hiddenImpl::FaceLayout<impl::Event>
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
        using hiddenImpl::FaceLayout<impl::Event>::pimpl;
        using hiddenImpl::FaceLayout<impl::Event>::impl;
    };

}}
