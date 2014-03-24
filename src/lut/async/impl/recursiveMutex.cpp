#include "lut/async/stable.hpp"
#include "lut/async/impl/recursiveMutex.hpp"

namespace lut { namespace async { namespace impl
{
    RecursiveMutex::RecursiveMutex()
        : _owner()
        , _ownerDepth()
    {
    }

    RecursiveMutex::~RecursiveMutex()
    {
        assert(!_owner);
    }

    void RecursiveMutex::acquire()
    {
        assert(0);
    }

    bool RecursiveMutex::tryAcquire()
    {
        assert(0);
    }

    void RecursiveMutex::release()
    {
        assert(0);
    }

    bool RecursiveMutex::isAcquired() const
    {
        assert(0);
    }

}}}
