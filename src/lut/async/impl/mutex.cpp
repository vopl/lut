#include "lut/async/stable.hpp"
#include "lut/async/impl/mutex.hpp"
#include "lut/async/impl/syncronizerWaiter.hpp"

namespace lut { namespace async { namespace impl
{
    Mutex::Mutex()
    {
    }

    Mutex::~Mutex()
    {
    }

    void Mutex::lock()
    {
        if(tryLock())
        {
            return;
        }

        SyncronizerPtr syncronizers[1] = {this};
        SyncronizerWaiter syncronizerWaiter(syncronizers, 1);

        syncronizerWaiter.all();
    }

    bool Mutex::tryLock()
    {
        if(!locked())
        {
            lock();
            return true;
        }

        return false;
    }

    void Mutex::unlock()
    {
        assert(locked());
        Syncronizer::unlock();
    }

    bool Mutex::locked() const
    {
        assert(0);
        return false;
    }

}}}
