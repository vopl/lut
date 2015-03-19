#include "connector.hpp"
#include "stream.hpp"
#include "../stream.hpp"
#include <stream.hpp>
#include <error.hpp>
#include "../utils/sockaddr.hpp"

#include <unistd.h>
#include <cstring>

namespace dci { namespace io { namespace impl { namespace fd
{

    Connector::Connector(Promise &&promise, const dci::io::Endpoint &endpoint)
        : Base {fd::etf_read|fd::etf_write|fd::etf_et}
        , _promise {std::forward<Promise>(promise)}
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
            _promise.setValue(io::make_error_code(io::error::general::not_implemented), dci::io::Stream());
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
            _promise.setValue(std::forward<std::error_code>(err), dci::io::Stream());
            delete this;
            return;
        }

        Stream *engine = new Stream;
        err = moveDescriptorTo(engine);

        if(err)
        {
            delete engine;

            _promise.setValue(std::forward<std::error_code>(err), dci::io::Stream());
            delete this;
            return;
        }

        dci::io::Stream stream;
        himpl::face2Impl(stream).setEngine(engine);

        _promise.setValue(std::error_code(), std::move(stream));
        delete this;
    }

}}}}
