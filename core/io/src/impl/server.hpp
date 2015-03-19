#pragma once

#include <endpoint.hpp>
#include <system_error>
#include <dci/async/future.hpp>
#include <stream.hpp>
#include <dci/mm/newDelete.hpp>


#include "fd/base.hpp"

namespace dci { namespace io { namespace impl
{

    class Server
        : fd::Base
    {
        Server(const Server &from) = delete;
        void operator=(const Server &from) = delete;

    public:
        Server();
        Server(Server &&from);
        ~Server();

        Server &operator=(Server &&from);

    public:
        std::error_code listen(const Endpoint &endpoint);
        async::Future<std::error_code, dci::io::Stream> accept();
        std::error_code close();

    private:
        template <typename Address>
        std::error_code listenInet(const Address &address, std::uint16_t port);

        std::error_code listenLocal(const io::str::Address &address);

    private:
        virtual void fdEvent(int typeFlags) override;
        virtual void fdClose() override;

    private:
        void flushError(const std::error_code &ec);

        bool makeAccept(async::Promise<std::error_code, io::Stream> &promise);

    private:
        bool _descriptorReady;

        struct Request
            : public mm::NewDelete<Request>
        {
            async::Promise<std::error_code, io::Stream> _promise;
            Request *_next;

            Request();
        };

        Request *_requestsFirst;
        Request *_requestsLast;

    };
}}}
