#include "lut/stable.hpp"
#include "lut/io/impl/server.hpp"
#include "lut/io/error.hpp"
#include "lut/io/impl/fd/stream.hpp"
#include "lut/io/impl/utils/sockaddr.hpp"
#include "lut/io/impl/stream.hpp"
#include "lut/io/endpoint.hpp"

namespace lut { namespace io { namespace impl
{
    Server::Request::Request()
        : _promise {}
        , _next {}
    {
    }

    Server::Server()
        : fd::Base {fd::etf_read}
        , _descriptorReady {}
        , _requestsFirst {}
        , _requestsLast {}
    {
    }

    Server::Server(Server &&from)
        : fd::Base {std::forward<fd::Base>(from)}
        , _descriptorReady {from._descriptorReady}
        , _requestsFirst {from._requestsFirst}
        , _requestsLast {from._requestsLast}
    {
        from._descriptorReady = false;
        from._requestsFirst = nullptr;
        from._requestsLast = nullptr;
    }

    Server::~Server()
    {
        flushError(make_error_code(error::stream::closed));
    }

    Server &Server::operator=(Server &&from)
    {
        assert(0);
    }

    std::error_code Server::listen(const Endpoint &endpoint)
    {
        if(getDescriptor() >= 0)
        {
            return make_error_code(error::general::secondary_listen);
        }

        switch(endpoint.scope())
        {
        case Scope::ipc:
            return listenLocal(endpoint.addressStr());
        case Scope::ip4:
            return listenInet(endpoint.addressIp4(), endpoint.port());
        case Scope::ip6:
            return listenInet(endpoint.addressIp6(), endpoint.port());
        default:
            assert(!"not impl");
            return io::make_error_code(io::error::general::not_implemented);
        }

        return io::make_error_code(io::error::general::not_implemented);
    }

    async::Future<std::error_code, lut::io::Stream> Server::accept()
    {
        if(getDescriptor() < 0)
        {
            return async::mkReadyFuture(io::make_error_code(error::general::no_listen), io::Stream());
        }

        if(_descriptorReady)
        {
            assert(!_requestsFirst);

            _descriptorReady = false;

            async::Promise<std::error_code, io::Stream> promise;
            if(makeAccept(promise))
            {
                return promise.future();
            }
        }

        if(_requestsFirst)
        {
            _requestsLast->_next = new Request {};
            _requestsLast = _requestsLast->_next;
        }
        else
        {
            _requestsFirst = _requestsLast = new Request {};
        }

        return _requestsLast->_promise.future();
    }

    std::error_code Server::close()
    {
        assert(0);
    }

    template <typename Address>
    std::error_code Server::listenInet(const Address &address, std::uint16_t port)
    {
        utils::Sockaddr<Address> sa;
        memset(&sa, 0, sizeof(sa));

        int family = utils::fillSockaddr(sa, address, port);

        int fd = socket(family, SOCK_STREAM|SOCK_NONBLOCK, 0);
        if(-1 == fd)
        {
            return std::error_code(errno, std::system_category());
        }

        {
            int optval = 1;
            int res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

            if(-1 == res)
            {
                ::close(fd);
                return std::error_code(errno, std::system_category());
            }
        }

        {
            int res = ::bind(fd, (sockaddr *)&sa, sizeof(sa));

            if(-1 == res)
            {
                ::close(fd);
                return std::error_code(errno, std::system_category());
            }
        }

        {
            std::error_code err = setDescriptor(fd);
            if(err)
            {
                ::close(fd);
                return err;
            }
        }

        {
            int res = ::listen(fd, 5);

            if(-1 == res)
            {
                setDescriptor(-1);
                ::close(fd);
                return std::error_code(errno, std::system_category());
            }
        }

        return std::error_code();
    }

    std::error_code Server::listenLocal(const io::str::Address &address)
    {
        assert(0);
    }

    void Server::fdEvent(int typeFlags)
    {
        if(fd::etf_error & typeFlags)
        {
            int errcode = ENOTSOCK;
            socklen_t errcodelen = sizeof(errcode);
            if(-1 == getsockopt(getDescriptor(), SOL_SOCKET, SO_ERROR, &errcode, &errcodelen))
            {
                errcode = errno;
            }

            flushError(std::error_code(errcode, std::system_category()));
            return;
        }

        if(fd::etf_read & typeFlags)
        {
            assert(!_descriptorReady);

            if(_requestsFirst)
            {
                Request *request = _requestsFirst;
                if(_requestsFirst == _requestsLast)
                {
                    _requestsFirst = nullptr;
                    _requestsLast = nullptr;
                }
                else
                {
                    _requestsFirst = request->_next;
                }

                bool b = makeAccept(request->_promise);
                assert(b);
                (void)b;

                delete request;
            }
            else
            {
                _descriptorReady = true;
            }

            return;
        }

        assert(!(fd::etf_write & typeFlags));
        assert(!"never here");
    }

    void Server::fdClose()
    {
        assert(0);
    }

    void Server::flushError(const std::error_code &ec)
    {
        assert(ec);

        if(std::error_condition(EAGAIN, std::system_category()) == ec)
        {
            assert(!"impossible");
            _descriptorReady = false;
        }
        else
        {
            _descriptorReady = false;

            Request *r = _requestsFirst;
            _requestsFirst = _requestsLast = nullptr;
            for(; r; r = r->_next)
            {
                r->_promise.setValue(std::error_code(ec), io::Stream {});
                delete r;
            }
        }
    }

    bool Server::makeAccept(async::Promise<std::error_code, io::Stream> &promise)
    {
        int acceptedDescriptor = ::accept4(getDescriptor(), nullptr, nullptr, SOCK_NONBLOCK);

        if(acceptedDescriptor >= 0)
        {
            fd::Stream *engine = new fd::Stream;
            std::error_code err = engine->setDescriptor(acceptedDescriptor);

            if(err)
            {
                delete engine;
                promise.setValue(std::forward<std::error_code>(err), lut::io::Stream());
                return false;
            }

            lut::io::Stream stream;
            himpl::face2Impl(stream).setEngine(engine);

            promise.setValue(std::error_code(), std::move(stream));
            return true;
        }

        flushError(std::error_code(errno, std::system_category()));
        return false;
    }

}}}
