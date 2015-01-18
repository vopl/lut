#pragma once

#include "lut/io/endpoint.hpp"
#include "lut/async/future.hpp"
#include "lut/io/stream.hpp"

#include <system_error>

namespace lut { namespace io
{
    namespace impl
    {
        class Server;
    }

    class Server
        : public hiddenImpl::Single<impl::Server>
    {
        Server(const Server &from) = delete;
        Server &operator=(const Server &from) = delete;

    public:
        Server();
        Server(Server &&from);
        ~Server();

        Server &operator=(Server &&from);

    public:
        std::error_code listen(const Endpoint &endpoint);
        async::Future<std::error_code, lut::io::Stream> accept();
        std::error_code close();
    };
}}
