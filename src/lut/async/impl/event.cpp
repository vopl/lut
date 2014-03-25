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

    bool Event::locked() const
    {
        assert(0);
    }

    void Event::lock()
    {
        assert(0);
    }

    bool Event::tryLock()
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

    bool Event::signalled() const
    {
        assert(0);
    }

}}}
