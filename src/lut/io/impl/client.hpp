#pragma once

#include "lut/async/future.hpp"
#include <system_error>
#include "lut/io/stream.hpp"
#include "lut/io/endpoint.hpp"

namespace lut { namespace io { namespace impl
{

    class Client
    {
        Client(const Client &) = delete;
        void operator=(const Client &) = delete;

    public:
        Client();
        Client(Client &&from);
        ~Client();

    public:
        async::Future<std::error_code, lut::io::Stream> connect(const lut::io::Endpoint &endpoint);

    };
}}}
