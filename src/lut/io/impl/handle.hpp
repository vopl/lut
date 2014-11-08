#pragma once


namespace lut { namespace io { namespace impl
{

    class Handle
    {
        Handle(const Handle &from) = delete;
        Handle &operator=(const Handle &from) = delete;

    public:
        Handle();
        ~Handle();

    public:
        //void close();

    };
}}}
