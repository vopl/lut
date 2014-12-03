#pragma once

#include "lut/hiddenImpl.hpp"
#include "lut/async/future.hpp"
#include "lut/io/stream.hpp"
#include "lut/io/endpoint.hpp"

#include <system_error>

namespace lut { namespace io
{
    namespace impl
    {
        class Client;
    }

    class Client
        : public HiddenImpl<impl::Client>
    {
        Client(const Client &from) = delete;
        Client &operator=(const Client &from) = delete;

    public:
        Client();
        ~Client();

    public:

        async::Future<std::error_code, Stream> connect(const Endpoint &endpoint);
    };
}}
