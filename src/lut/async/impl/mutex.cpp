#include "lut/async/stable.hpp"
#include "lut/async/impl/mutex.hpp"

namespace lut { namespace async { namespace impl
{
    Mutex::Mutex()
    {
    }

    Mutex::~Mutex()
    {
    }

    void Mutex::acquire()
    {
        assert(0);
    }

    bool Mutex::tryAcquire()
    {
        assert(0);
    }

    void Mutex::release()
    {
        assert(0);
    }

    bool Mutex::isAcquired() const
    {
        assert(0);
    }

}}}
