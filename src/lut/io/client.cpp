#include "lut/stable.hpp"
#include "lut/io/client.hpp"
#include "lut/io/impl/client.hpp"
#include "lut/io/impl/loop.hpp"
#include "lut/io/impl/fd/tcpConnector.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace lut { namespace io
{

    Client::Client()
        : HiddenImpl<impl::Client>()
    {
    }

    Client::~Client()
    {
    }

    namespace
    {
        async::Future<std::error_code, Stream> connectIp(const ip6::Address &address, std::uint16_t port)
        {
            assert(0);
        }

        void cb(void *userData, int events)
        {

        }

//        async::Future<std::error_code, Stream> connectIp(const ip4::Address &address, std::uint16_t port)
//        {
//            async::Promise<std::error_code, Stream> promise;

//            io::impl::fd::Tcp *fdb = new io::impl::fd::Tcp;

//            std::error_code err0 = fdb->connect();
//            if(err0)
//            {
//                std::cout << err0.message() << std::endl;
//            }


//            sockaddr_in addr;
//            addr.sin_family = AF_INET;
//            addr.sin_port = htons(port);
//            addr.sin_addr.s_addr = htonl(address._value._uint32);
//            int res = connect(fd, (sockaddr *)&addr, sizeof(addr));

//            if(0 == res)
//            {
//                Stream stream;
//                //stream.setFd(fd);
//                promise.setValue(std::error_code(), std::move(stream));
//                return promise.future();
//            }

//            std::error_code err(errno, std::system_category());

//            std::cout << err.message() << std::endl;

//            promise.setValue(std::error_code(), Stream());
//            return promise.future();
//        }

        async::Future<std::error_code, Stream> connectIpc(const str::Address &address)
        {
            assert(0);
        }

    }

    async::Future<std::error_code, Stream> Client::connect(const Endpoint &endpoint)
    {
        assert(Scope::ip4==endpoint.scope() || Scope::ip6==endpoint.scope());

        switch(endpoint.scope())
        {
        case Scope::ip6:
            return connectIp(endpoint.addressIp6(), endpoint.port());
        case Scope::ip4:
            {
                io::impl::fd::TcpConnector *connector = new io::impl::fd::TcpConnector;
                async::Future<std::error_code, Stream> res = connector->getFuture();
                connector->start(endpoint.addressIp4(), endpoint.port());
                return res;
            }
        case Scope::ipc:
            return connectIpc(endpoint.addressStr());
        default:
            assert(0);
        }

        assert(0);
    }
}}
