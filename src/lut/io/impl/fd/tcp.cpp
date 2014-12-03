
#include "lut/stable.hpp"
#include "lut/io/impl/fd/tcp.hpp"
#include "lut/io/stream.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace lut { namespace io { namespace impl { namespace fd
{

    Tcp::Tcp()
        : Base()
    {

    }

    Tcp::~Tcp()
    {
        close();
    }

    void Tcp::event(int typeFlags)
    {
        if(etf_error & typeFlags)
        {
            assert(0);
        }
        if(etf_read & typeFlags)
        {
            assert(0);
        }
        if(etf_write & typeFlags)
        {
            assert(0);
        }
    }

    void Tcp::close()
    {
        int descriptor = getDescriptor();
        if(-1 != descriptor)
        {
            setDescriptor(-1);
            ::close(descriptor);
        }
    }

}}}}
