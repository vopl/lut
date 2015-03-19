#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/async/future.hpp>
#include "stream.hpp"
#include "endpoint.hpp"

#include <system_error>

namespace dci { namespace io
{
    namespace impl
    {
        class Client;
    }

    class Client
        : public himpl::FaceLayout<impl::Client>
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
