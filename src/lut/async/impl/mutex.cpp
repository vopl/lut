#include "lut/async/stable.hpp"
#include "lut/async/impl/mutex.hpp"
#include "lut/async/impl/acquireWaiter.hpp"

namespace lut { namespace async { namespace impl
{
    Mutex::Mutex()
    {
    }

    Mutex::~Mutex()
    {
    }

    void Mutex::acquire()
    {
        if(tryAcquire())
        {
            return;
        }

        SyncronizerPtr syncronizers[1] = {this};
        AcquireWaiter acquireWaiter(syncronizers, 1);

        acquireWaiter.all();
    }

    bool Mutex::tryAcquire()
    {
        if(!_acqiured)
        {
            _acqiured = true;
            return true;
        }

        return false;
    }

    void Mutex::release()
    {
        assert(_acqiured);
        _acqiured = false;
    }

    bool Mutex::isAcquired() const
    {
        return _acqiured;
    }

}}}
