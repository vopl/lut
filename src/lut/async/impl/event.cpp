#include "lut/async/stable.hpp"
#include "lut/async/impl/event.hpp"

namespace lut { namespace async { namespace impl
{
    Event::Event(bool autoReset)
        : _autoReset(autoReset)
        , _state()
    {
    }

    Event::~Event()
    {
    }

    void Event::acquire()
    {
        assert(0);
    }

    bool Event::tryAcquire()
    {
        assert(0);
    }

    void Event::set()
    {
        assert(0);
    }

    void Event::reset()
    {
        assert(0);
    }

    bool Event::isSignalled() const
    {
        assert(0);
    }

}}}
