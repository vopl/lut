
#include "lut/stable.hpp"
#include "lut/io/impl/fd/tcpConnector.hpp"
#include "lut/io/impl/fd/stream.hpp"
#include "lut/io/impl/stream.hpp"
#include "lut/io/stream.hpp"
#include "lut/io/error.hpp"
#include "lut/hiddenImpl/accessor.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace lut { namespace io { namespace impl { namespace fd
{

    TcpConnector::TcpConnector()
        : Base()
    {

    }

    TcpConnector::~TcpConnector()
    {
        close();
    }

    async::Future<std::error_code, lut::io::Stream> TcpConnector::getFuture()
    {
        return _promise.future();
    }

    void TcpConnector::start(const ip4::Address &address, std::uint16_t port)
    {
        int fd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
        if(-1 == fd)
        {
            resolve(std::error_code(errno, std::system_category()));
            return;
        }

        std::error_code err = setDescriptor(fd);
        if(err)
        {
            resolve(err);
            return;
        }

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(address._value._uint32);
        int res = ::connect(fd, (sockaddr *)&addr, sizeof(addr));

        if(0 == res)
        {
            //sync
            resolve(std::error_code());
            return;
        }
        else if(EINPROGRESS == errno)
        {
            //async
            return;
        }

        //error
        resolve(std::error_code(errno, std::system_category()));
    }

    void TcpConnector::event(int typeFlags)
    {
        assert(! (typeFlags & etf_read));

        if(typeFlags & etf_write)
        {
            resolve(std::error_code());
            return;
        }

        if(typeFlags & etf_error)
        {
            int errcode = ENOTSOCK;
            socklen_t errcodelen = sizeof(errcode);
            if(-1 == getsockopt(getDescriptor(), SOL_SOCKET, SO_ERROR, &errcode, &errcodelen))
            {
                errcode = errno;
            }

            resolve(std::error_code(errcode, std::system_category()));
            return;
        }

        assert(!"not impossible");
    }

    void TcpConnector::close()
    {
        int fd = getDescriptor();
        if(-1 == fd)
        {
            assert(_promise.isReady());
            return;
        }

        setDescriptor(-1);
        ::close(fd);

        assert(!_promise.isReady());
        if(!_promise.isReady())
        {
            resolve(make_error_code(error::stream::closed));
        }
    }

    void TcpConnector::resolve(std::error_code err)
    {
        if(err)
        {
            _promise.setValue(std::forward<std::error_code>(err), lut::io::Stream());
            delete this;
            return;
        }

        Stream *engine = new Stream;
        err = moveDescriptorTo(engine);

        if(err)
        {
            delete engine;

            _promise.setValue(std::forward<std::error_code>(err), lut::io::Stream());
            delete this;
            return;
        }

        lut::io::Stream stream;
        hiddenImpl::Accessor::access(stream).setEngine(engine);

        _promise.setValue(std::error_code(), std::move(stream));
        delete this;
    }

}}}}
