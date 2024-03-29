
#include "stream.hpp"
#include <stream.hpp>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

namespace dci { namespace io { namespace impl { namespace fd
{

    Stream::Stream()
        : Base {fd::etf_read|fd::etf_write|fd::etf_et}
        , _reader {}
        , _writer {}
    {
    }

    Stream::~Stream()
    {
        fdClose();
    }

    async::Future<std::error_code, io::Data> Stream::read(std::size_t min, std::size_t max)
    {
        return _reader.read(getDescriptor(), min, max);
    }

    async::Future<std::error_code> Stream::write(io::Data &&data)
    {
        return _writer.write(getDescriptor(), std::forward<io::Data>(data));
    }

    void Stream::fdEvent(int typeFlags)
    {
        if(etf_error & typeFlags)
        {
            int errcode = ENOTSOCK;
            socklen_t errcodelen = sizeof(errcode);
            if(-1 == getsockopt(getDescriptor(), SOL_SOCKET, SO_ERROR, &errcode, &errcodelen))
            {
                errcode = errno;
            }

            std::error_code err(errcode, std::system_category());
            _reader.error(err);
            _writer.error(err);
        }
        if(etf_read & typeFlags)
        {
            _reader.ready(getDescriptor());
        }
        if(etf_write & typeFlags)
        {
            _writer.ready(getDescriptor());
        }
    }

    void Stream::fdClose()
    {
        int descriptor = getDescriptor();
        if(-1 != descriptor)
        {
            setDescriptor(-1);
            _reader.close();
            _writer.close();
            ::close(descriptor);
        }
    }
}}}}
