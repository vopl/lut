#include "lut/async/stable.hpp"
#include "lut/async/event.hpp"
#include "lut/async/impl/event.hpp"

namespace lut { namespace async
{
    Event::Event(bool autoReset)
        : HiddenImpl<impl::Event>(autoReset)
    {

    }

    Event::~Event()
    {

    }

    void Event::acquire()
    {
        return impl().acquire();
    }

    bool Event::tryAcquire()
    {
        return impl().tryAcquire();
    }

    void Event::set()
    {
        return impl().set();
    }

    void Event::reset()
    {
        return impl().reset();
    }

    bool Event::isSignalled() const
    {
        return impl().isSignalled();
    }

}}
