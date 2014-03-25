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

    void Mutex::lock()
    {
        return impl().lock();
    }

    bool Mutex::tryLock()
    {
        return impl().tryLock();
    }

    void Mutex::unlock()
    {
        return impl().unlock();
    }

    bool Mutex::locked() const
    {
        return impl().locked();
    }

}}
