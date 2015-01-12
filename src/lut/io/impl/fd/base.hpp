#pragma once

#include <system_error>

namespace lut { namespace io { namespace impl { namespace fd
{
    class Base;
}}}}

namespace lut { namespace io { namespace impl { namespace loop
{
    fd::Base *&next(fd::Base*);
    fd::Base *&prev(fd::Base*);
}}}}

namespace lut { namespace io { namespace impl { namespace fd
{

    enum EventTypeFlag
    {
        etf_read    = 0x01,
        etf_write   = 0x02,
        etf_error   = 0x04,
    };

    class Base
    {
        Base(const Base &) = delete;

    public:
        Base();
        Base(Base &&from);
        virtual ~Base();

        std::error_code setDescriptor(int descriptor);
        std::error_code moveDescriptorTo(Base *to);
        int getDescriptor();

        virtual void fdEvent(int typeFlags) = 0;
        virtual void fdClose() = 0;

    private:
        int _descriptor;

    private:
        friend Base *&loop::next(Base*);
        friend Base *&loop::prev(Base*);

        Base *_nextOnPoller;
        Base *_prevOnPoller;
    };

}}}}
