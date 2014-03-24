#include "lut/async/stable.hpp"
#include "lut/async/impl/semaphore.hpp"

#include <cstdint>

namespace lut { namespace async { namespace impl
{
    Semaphore::Semaphore(std::size_t n)
        : _counter(n)
    {
    }

    Semaphore::~Semaphore()
    {
    }

    void Semaphore::acquire()
    {
        assert(0);
    }

    bool Semaphore::tryAcquire()
    {
        assert(0);
    }

    void Semaphore::release()
    {
        assert(0);
    }

    std::size_t Semaphore::counter() const
    {
        assert(0);
    }

}}}
