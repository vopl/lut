#pragma once

#include <dci/async/future.hpp>
#include "base.hpp"
#include <endpoint.hpp>
#include <dci/mm/newDelete.hpp>

#include <system_error>

namespace dci { namespace io
{
    class Stream;
}}

namespace dci { namespace io { namespace impl { namespace fd
{

    class Connector
        : public Base
        , public mm::NewDelete<Connector>
    {
        using Promise = async::Promise<std::error_code, dci::io::Stream>;

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
