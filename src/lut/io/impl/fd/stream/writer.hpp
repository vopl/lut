#pragma once

#include "lut/async/future.hpp"
#include "lut/io/data.hpp"
#include "lut/mm/newDelete.hpp"

#include <system_error>

namespace lut { namespace io { namespace impl { namespace fd { namespace stream
{

    class Writer
    {
    public:
        Writer();
        ~Writer();

        void error(const std::error_code &err);
        void ready(int descriptor);
        async::Future<std::error_code> write(int descriptor, io::Data &&data);
        void close();

    private:
        void pump(int descriptor);
        void flushProcessed(std::size_t size);
        void flushError(const std::error_code &ec);

    private:
        bool _descriptorReady;

        io::Data _buffer;

        struct Request
            : public mm::NewDelete<Request>
        {
            std::size_t _tailSize;
            async::Promise<std::error_code> _promise;
            Request *_next;

            Request(std::size_t tailSize);
        };

        Request *_requestsFirst;
        Request *_requestsLast;
    };

}}}}}
