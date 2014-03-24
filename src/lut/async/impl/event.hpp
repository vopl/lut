#ifndef _LUT_ASYNC_IMPL_EVENT_HPP_
#define _LUT_ASYNC_IMPL_EVENT_HPP_

#include "lut/async/impl/syncronizer.hpp"

namespace lut { namespace async { namespace impl
{

    class Event
        : public Syncronizer
    {
    public:
        Event(bool autoReset);
        ~Event();

    public:
        void acquire();
        bool tryAcquire();

    public:
        void set();
        void reset();

    public:
        bool isSignalled() const;

    private:
        bool _autoReset;
        bool _state;

    };

}}}

#endif
