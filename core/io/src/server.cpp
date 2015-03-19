#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE(<sizeProvider.hpp>)
#include <server.hpp>
#include "impl/server.hpp"

namespace dci { namespace io
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

    async::Future<std::error_code, dci::io::Stream> Server::accept()
    {
        return impl().accept();
    }

    std::error_code Server::close()
    {
        return impl().close();
    }

}}
