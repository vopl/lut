#pragma once

namespace lut { namespace io
{
    namespace impl
    {
        class Server;
    }

    class Server
        : public HiddenImpl<impl::Server>
    {
        Server(const Server &from) = delete;
        Server &operator=(const Server &from) = delete;

    public:
        Server();
        ~Server();

    public:

        Error bind(const Endpoint &endpoint);
        async::Future<Stream> accept();
    };
}}
