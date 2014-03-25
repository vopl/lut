#include "lut/async/stable.hpp"
#include "lut/async/impl/event.hpp"

namespace lut { namespace async { namespace impl
{
    Event::Event(bool autoReset)
        : _autoReset(autoReset)
        , _signalled()
    {
    }

    Event::~Event()
    {
    }

    bool Event::locked() const
    {
        return !_signalled;
    }

    void Event::lock()
    {
        if(tryLock())
        {
            return;
        }

        SyncronizerPtr syncronizers[1] = {this};
        SyncronizerWaiter syncronizerWaiter(syncronizers, 1);

        syncronizerWaiter.all();
    }

    bool Event::tryLock()
    {
        if(_signalled)
        {
            if(_autoReset)
            {
                return true;
            }
            _signalled = false;
            return true;
        }

        return false;
    }

    void Event::set()
    {
        if(!_signalled)
        {
            _signalled = true;
            Syncronizer::unlock();
        }
    }

    void Event::reset()
    {
        _signalled = false;
    }

    bool Event::signalled() const
    {
        return _signalled;
    }

}}}
