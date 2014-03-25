#include "lut/async/stable.hpp"
#include "lut/async/impl/mutex.hpp"
#include "lut/async/impl/syncronizerWaiter.hpp"

namespace lut { namespace async { namespace impl
{
    Mutex::Mutex()
        : _locked()
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
            _locked = true;
            return true;
        }

        return false;
    }

    void Mutex::unlock()
    {
        assert(locked());
        _locked = false;
        Syncronizer::unlock();
    }

    bool Mutex::locked() const
    {
        return _locked;
    }

}}}
