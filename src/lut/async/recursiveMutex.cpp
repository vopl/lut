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

    void RecursiveMutex::lock()
    {
        return impl().lock();
    }

    bool RecursiveMutex::tryLock()
    {
        return impl().tryLock();
    }

    void RecursiveMutex::unlock()
    {
        return impl().unlock();
    }

    bool RecursiveMutex::locked() const
    {
        return impl().locked();
    }

}}
