#include "lut/stable.hpp"
#include "lut/io/impl/client.hpp"
#include "lut/io/impl/fd/connector.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace lut { namespace io { namespace impl
{
    Client::Client()
    {
    }

    Client::Client(Client &&from)
    {
        (void)from;
    }

    Client::~Client()
    {
    }

    async::Future<std::error_code, lut::io::Stream> Client::connect(const lut::io::Endpoint &endpoint)
    {
        async::Promise<std::error_code, lut::io::Stream> promise;
        async::Future<std::error_code, lut::io::Stream> res = promise.future();
        new io::impl::fd::Connector(std::move(promise), endpoint);
        return res;
    }
}}}
