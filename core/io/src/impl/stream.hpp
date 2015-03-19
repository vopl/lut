#pragma once

#include <dci/async/future.hpp>
#include <data.hpp>
#include <system_error>

namespace dci { namespace io { namespace impl
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

    public:
        async::Future<std::error_code, io::Data> read(std::size_t min, std::size_t max);
        async::Future<std::error_code> write(io::Data &&data);

        void close();

    private:

        union
        {
            fd::Stream *_fd;
        } _engine;

        enum class EngineType
        {
            null,
            fd,
        } _engineType;
    };
}}}
