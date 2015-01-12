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

    class Connector
        : public Base
    {
        using Promise = async::Promise<std::error_code, lut::io::Stream>;

    public:
        Connector(Promise &&promise, const Endpoint &endpoint);
        ~Connector();

    private:
        template <typename Address>
        void startInet(const Address &address, std::uint16_t port);

        void startLocal(const io::str::Address &address);

    private:
        virtual void fdEvent(int typeFlags);
        virtual void fdClose();

    private:
        void resolve(std::error_code err);

    private:
        Promise _promise;
    };
}}}}
