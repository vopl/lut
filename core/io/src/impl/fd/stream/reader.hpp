#pragma once

#include <dci/async/future.hpp>
#include <data.hpp>
#include "../../data/segment.hpp"
#include <dci/mm/newDelete.hpp>

#include <system_error>
#include <vector>

namespace dci { namespace io { namespace impl { namespace fd { namespace stream
{

    class Reader
    {
    public:
        Reader();
        ~Reader();

        void error(const std::error_code &err);
        void ready(int descriptor);
        async::Future<std::error_code, io::Data> read(int descriptor, std::size_t min, std::size_t max);
        void close();

    private:
        void flushProcessed(impl::Data &&data);
        void flushError(const std::error_code &ec);

        void pump(int descriptor);


    private:
        bool _descriptorReady;
        io::Data _buffer;

        struct Request
            : public mm::NewDelete<Request>
        {
            std::size_t _needMinSize;
            std::size_t _needMaxSize;

            async::Promise<std::error_code, io::Data> _promise;
            Request *_next;

            Request(std::size_t needMinSize, std::size_t needMaxSize);
        };

        Request *_requestsFirst;
        Request *_requestsLast;

    private:
        class PreparedBuffer
        {
        public:
            PreparedBuffer();
            ~PreparedBuffer();
            std::pair<iovec *, std::size_t> prepare(std::size_t size);
            impl::Data flush(std::size_t size);

        private:
            impl::data::Segment *   _segmentFirst;
            impl::data::Segment *   _segmentLast;
            std::vector<iovec>      _iovecs;
        };

        static PreparedBuffer _preparedBuffer;

    };

}}}}}
