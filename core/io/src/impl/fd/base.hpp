#pragma once

#include <system_error>

namespace dci { namespace io { namespace impl { namespace fd
{
    class Base;
}}}}

namespace dci { namespace io { namespace impl { namespace loop
{
    fd::Base *&next(fd::Base*);
    fd::Base *&prev(fd::Base*);
}}}}

namespace dci { namespace io { namespace impl { namespace fd
{

    enum EventTypeFlag
    {
        etf_read    = 0x01,
        etf_write   = 0x02,

        etf_et      = 0x10,

        etf_error   = 0x100,
    };

    class Base
    {
        Base(const Base &) = delete;

    public:
        Base(int eventTypes/* = etf_read | etf_write | etf_et*/);
        Base(Base &&from);
        virtual ~Base();

        std::error_code setDescriptor(int descriptor);
        std::error_code moveDescriptorTo(Base *to);

        int getDescriptor() const;
        int getEventTypes() const;

        virtual void fdEvent(int typeFlags) = 0;
        virtual void fdClose() = 0;

    private:
        int _descriptor;
        int _eventTypes;

    private:
        friend Base *&loop::next(Base*);
        friend Base *&loop::prev(Base*);

        Base *_nextOnPoller;
        Base *_prevOnPoller;
    };

}}}}
