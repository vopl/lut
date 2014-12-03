#include "lut/stable.hpp"
#include "lut/io/impl/fd/base.hpp"
#include "lut/io/impl/loop.hpp"

namespace lut { namespace io { namespace impl { namespace fd
{

    Base::Base()
        : _descriptor{-1}
        , _nextOnPoller{}
        , _prevOnPoller{}

    {
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
            return loop::listenerAdd(this);
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


    int Base::getDescriptor()
    {
        return _descriptor;
    }

    void Base::event(int typeFlags)
    {
        (void)typeFlags;
        assert(!"never here");
        abort();
    }

}}}}
