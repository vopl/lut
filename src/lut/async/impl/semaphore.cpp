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

    bool Semaphore::locked() const
    {
        assert(0);
    }

    void Semaphore::lock()
    {
        if(tryLock())
        {
            return;
        }

        SyncronizerPtr syncronizers[1] = {this};
        SyncronizerWaiter syncronizerWaiter(syncronizers, 1);

        syncronizerWaiter.all();
    }

    bool Semaphore::tryLock()
    {
        if(_counter)
        {
            assert(!locked());

            _counter--;
            return true;
        }

        return false;
    }

    void Semaphore::unlock()
    {
        _counter++;
        if(1 == _counter)
        {
            assert(locked());
            Syncronizer::unlock();
        }
    }

    std::size_t Semaphore::counter() const
    {
        return _counter;
    }

}}}
