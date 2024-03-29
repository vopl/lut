#include <event.hpp>
#include "impl/event.hpp"

namespace dci { namespace async
{
    Event::Event(bool autoReset)
        : himpl::FaceLayout<impl::Event>(autoReset)
    {

    }

    Event::~Event()
    {

    }

    void Event::acquire()
    {
        return impl().lock();
    }

    bool Event::tryAcquire()
    {
        return impl().tryLock();
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
        return !impl().locked();
    }

}}
