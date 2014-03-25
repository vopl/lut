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

    void RecursiveMutex::lock()
    {
        assert(0);
    }

    bool RecursiveMutex::tryLock()
    {
        assert(0);
    }

    void RecursiveMutex::unlock()
    {
        assert(0);
    }

    bool RecursiveMutex::locked() const
    {
        assert(0);
    }

}}}
