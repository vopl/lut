#pragma once

#include "lut/async/future.hpp"
#include "lut/io/impl/fd/base.hpp"
#include "lut/io/endpoint.hpp"

#include <system_error>

namespace lut { namespace io
{
    class Stream;
}}

namespace lut { namespace io { namespace impl { namespace fd
{

    class TcpConnector
        : public Base
    {
    public:
        TcpConnector();
        ~TcpConnector();

        async::Future<std::error_code, lut::io::Stream> getFuture();

        void start(const ip4::Address &address, uint16_t port);

    private:
        virtual void event(int typeFlags);
        virtual void close();

    private:
        void resolve(std::error_code err);

    private:
        async::Promise<std::error_code, lut::io::Stream> _promise;
    };
}}}}
