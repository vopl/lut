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

    void Semaphore::enter()
    {
        return impl().lock();
    }

    bool Semaphore::tryEnter()
    {
        return impl().tryLock();
    }
    void Semaphore::leave()
    {
        return impl().unlock();
    }
    std::size_t Semaphore::counter() const
    {
        return impl().counter();
    }

}}
