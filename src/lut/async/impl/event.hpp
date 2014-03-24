#ifndef _LUT_ASYNC_IMPL_EVENT_HPP_
#define _LUT_ASYNC_IMPL_EVENT_HPP_

namespace lut { namespace async { namespace impl
{

    class Event
    {
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
    };

}}}

#endif
