#include "lut/async/stable.hpp"
#include "lut/async/recursiveMutex.hpp"
#include "lut/async/impl/recursiveMutex.hpp"

namespace lut { namespace async
{
    RecursiveMutex::RecursiveMutex()
        : HiddenImpl<impl::RecursiveMutex>()
    {

    }

    RecursiveMutex::~RecursiveMutex()
    {

    }

    void RecursiveMutex::acquire()
    {
        return impl().acquire();
    }

    bool RecursiveMutex::tryAcquire()
    {
        return impl().tryAcquire();
    }

    void RecursiveMutex::release()
    {
        return impl().release();
    }

    bool RecursiveMutex::isAcquired() const
    {
        return impl().isAcquired();
    }

}}
