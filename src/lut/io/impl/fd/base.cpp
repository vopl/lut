#include "lut/stable.hpp"
#include "lut/io/impl/fd/base.hpp"
#include "lut/io/impl/loop.hpp"

namespace lut { namespace io { namespace impl { namespace fd
{

    Base::Base(int eventTypes)
        : _descriptor{-1}
        , _eventTypes{eventTypes}
        , _nextOnPoller{}
        , _prevOnPoller{}
    {
    }

    Base::Base(Base &&from)
        : _descriptor{from._descriptor}
        , _eventTypes{from._eventTypes}
        , _nextOnPoller{from._nextOnPoller}
        , _prevOnPoller{from._prevOnPoller}
    {
        from._descriptor = -1;
        from._eventTypes = 0;
        from._nextOnPoller = nullptr;
        from._prevOnPoller = nullptr;
    }

    Base::~Base()
    {
        if(_descriptor >= 0)
        {
            loop::listenerDel(this);
        }
    }

    std::error_code Base::setDescriptor(int descriptor)
    {
        if(_descriptor >= 0)
        {
            std::error_code err = loop::listenerDel(this);

            if(err)
            {
                return err;
            }
        }

        _descriptor = descriptor;

        if(_descriptor >= 0)
        {
            std::error_code err = loop::listenerAdd(this);

            if(err)
            {
                _descriptor = -1;
                return err;
            }
        }

        return std::error_code();
    }

    std::error_code Base::moveDescriptorTo(Base *to)
    {
        assert(-1 != _descriptor);
        assert(-1 == to->_descriptor);
        to->_descriptor = _descriptor;

        std::error_code res = loop::listenerReplace(this, to);

        if(res)
        {
            to->_descriptor = -1;
        }
        else
        {
            _descriptor = -1;
        }

        return res;
    }


    int Base::getDescriptor() const
    {
        return _descriptor;
    }

    int Base::getEventTypes() const
    {
        return _eventTypes;
    }

    void Base::fdEvent(int typeFlags)
    {
        (void)typeFlags;
        assert(!"never here");
        abort();
    }

}}}}
