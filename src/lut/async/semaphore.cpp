#include "lut/async/stable.hpp"
#include "lut/async/semaphore.hpp"
#include "lut/async/impl/semaphore.hpp"

namespace lut { namespace async
{
    Semaphore::Semaphore(std::size_t n)
        : HiddenImpl<impl::Semaphore>(n)
    {

    }

    Semaphore::~Semaphore()
    {

    }

    void Semaphore::acquire()
    {
        return impl().acquire();
    }

    bool Semaphore::tryAcquire()
    {
        return impl().tryAcquire();
    }
    void Semaphore::release()
    {
        return impl().release();
    }
    std::size_t Semaphore::counter() const
    {
        return impl().counter();
    }

}}
