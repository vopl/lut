
#include "lut/stable.hpp"
#include "lut/io/impl/fd/connector.hpp"
#include "lut/io/impl/fd/stream.hpp"
#include "lut/io/impl/stream.hpp"
#include "lut/io/stream.hpp"
#include "lut/io/error.hpp"
#include "lut/hiddenImpl/accessor.hpp"
#include "lut/io/impl/utils/sockaddr.hpp"

namespace lut { namespace io { namespace impl { namespace fd
{

    Connector::Connector(Promise &&promise, const lut::io::Endpoint &endpoint)
        : Base()
        , _promise(std::forward<Promise>(promise))
    {
        switch(endpoint.scope())
        {
        case Scope::ipc:
            startLocal(endpoint.addressStr());
            return;
        case Scope::ip4:
            startInet(endpoint.addressIp4(), endpoint.port());
            return;
        case Scope::ip6:
            startInet(endpoint.addressIp6(), endpoint.port());
            return;
        default:
            assert(!"not impl");
            _promise.setValue(io::make_error_code(io::error::general::not_implemented), lut::io::Stream());
            delete this;
            return;
        }
    }

    Connector::~Connector()
    {
        fdClose();
    }

    template <typename Address>
    void Connector::startInet(const Address &address, std::uint16_t port)
    {
        utils::Sockaddr<Address> sa;
        memset(&sa, 0, sizeof(sa));

        int family = utils::fillSockaddr(sa, address, port);

        int fd = socket(family, SOCK_STREAM|SOCK_NONBLOCK, 0);
        if(-1 == fd)
        {
            resolve(std::error_code(errno, std::system_category()));
            return;
        }

        std::error_code err = setDescriptor(fd);
        if(err)
        {
            close(fd);
            resolve(err);
            return;
        }

        int res = ::connect(fd, (sockaddr *)&sa, sizeof(sa));

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

    void Connector::startLocal(const io::str::Address &address)
    {
        assert(!"not impl");
    }

    void Connector::fdEvent(int typeFlags)
    {
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

        assert(! (typeFlags & etf_read));

        if(typeFlags & etf_write)
        {
            resolve(std::error_code());
            return;
        }

        assert(!"not impossible");
    }

    void Connector::fdClose()
    {
        int fd = getDescriptor();
        if(-1 == fd)
        {
            assert(_promise.isReady());
            return;
        }

        setDescriptor(-1);
        ::close(fd);

        if(!_promise.isReady())
        {
            resolve(make_error_code(error::stream::closed));
        }
    }

    void Connector::resolve(std::error_code err)
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
