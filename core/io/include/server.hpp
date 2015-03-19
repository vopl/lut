#pragma once

#include <endpoint.hpp>
#include <dci/async/future.hpp>
#include <stream.hpp>

#include <system_error>

namespace dci { namespace io
{
    namespace impl
    {
        class Server;
    }

    class Server
        : public himpl::FaceLayout<impl::Server>
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
        async::Future<std::error_code, dci::io::Stream> accept();
        std::error_code close();
    };
}}
