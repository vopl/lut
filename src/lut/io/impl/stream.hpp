#pragma once

#include "lut/async/future.hpp"
#include "lut/io/data.hpp"
#include <system_error>

namespace lut { namespace io { namespace impl
{

    namespace fd
    {
        class Tcp;
    }

    class Stream
    {

    public:
        Stream();

        Stream(Stream &&from);
        ~Stream();

        void setEngine(fd::Tcp *tcpEngine);

        async::Future<std::error_code> shutdown(bool read, bool write);

        async::Future<std::error_code, Data> read(int min, int max);
        std::error_code write(const Data &data);

        void close();

        bool isReadable();
        bool isWriteable();

    private:

        union
        {
            fd::Tcp *_tcp;
        } _engine;

        enum class EngineType
        {
            null,
            tcp,
        } _engineType;
    };
}}}
