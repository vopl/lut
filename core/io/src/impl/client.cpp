#include "client.hpp"
#include "fd/connector.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace dci { namespace io { namespace impl
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

    async::Future<std::error_code, dci::io::Stream> Client::connect(const dci::io::Endpoint &endpoint)
    {
        async::Promise<std::error_code, dci::io::Stream> promise;
        async::Future<std::error_code, dci::io::Stream> res = promise.future();
        new io::impl::fd::Connector(std::move(promise), endpoint);
        return res;
    }
}}}
