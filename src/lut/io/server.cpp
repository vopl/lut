#include "lut/stable.hpp"
#include "lut/io/sizeProvider.hpp"
#include "lut/io/server.hpp"
#include "lut/io/impl/server.hpp"

namespace lut { namespace io
{

    Server::Server()
        : himpl::FaceLayout<impl::Server>()
    {
    }

    Server::Server(Server &&from)
        : himpl::FaceLayout<impl::Server>(std::forward<himpl::FaceLayout<impl::Server>>(from))
    {
    }

    Server::~Server()
    {
    }

    Server &Server::operator=(Server &&from)
    {
        impl() = std::forward<impl::Server>(from.impl());
        return *this;
    }

    std::error_code Server::listen(const Endpoint &endpoint)
    {
        return impl().listen(endpoint);
    }

    async::Future<std::error_code, lut::io::Stream> Server::accept()
    {
        return impl().accept();
    }

    std::error_code Server::close()
    {
        return impl().close();
    }

}}
