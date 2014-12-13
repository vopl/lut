#pragma once

#include "lut/async/future.hpp"
#include "lut/io/data.hpp"
#include <system_error>

namespace lut { namespace io { namespace impl
{

    namespace fd
    {
        class Stream;
    }

    class Stream
    {

    public:
        Stream();

        Stream(Stream &&from);
        ~Stream();

        void setEngine(fd::Stream *tcpEngine);

        async::Future<std::error_code, io::Data> read(std::size_t min, std::size_t max);
        async::Future<std::error_code> write(io::Data &&data);

        void close();

    private:

        union
        {
            fd::Stream *_tcp;
        } _engine;

        enum class EngineType
        {
            null,
            tcp,
        } _engineType;
    };
}}}
