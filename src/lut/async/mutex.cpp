#include "lut/async/stable.hpp"
#include "lut/async/mutex.hpp"
#include "lut/async/impl/mutex.hpp"

namespace lut { namespace async
{
    Mutex::Mutex()
        : HiddenImpl<impl::Mutex>()
    {

    }

    Mutex::~Mutex()
    {

    }

    void Mutex::acquire()
    {
        return impl().acquire();
    }

    bool Mutex::tryAcquire()
    {
        return impl().tryAcquire();
    }

    void Mutex::release()
    {
        return impl().release();
    }

    bool Mutex::isAcquired() const
    {
        return impl().isAcquired();
    }

}}
