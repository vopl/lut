#include "lut/io/impl/stream.hpp"
#include "lut/io/impl/fd/tcp.hpp"

namespace lut { namespace io { namespace impl
{
    Stream::Stream()
        : _engineType{EngineType::null}
    {
    }

    Stream::Stream(Stream &&from)
        : _engineType{from._engineType}
    {
        switch(_engineType)
        {
        case EngineType::null:
            return;
        case EngineType::tcp:
            _engine._tcp = from._engine._tcp;
            from._engineType = EngineType::null;
            return;
        }
    }

    Stream::~Stream()
    {
        close();
    }

    void Stream::setEngine(fd::Tcp *tcpEngine)
    {
        close();

        _engine._tcp = tcpEngine;
        _engineType = EngineType::tcp;
    }

    async::Future<std::error_code> Stream::shutdown(bool read, bool write)
    {
        assert(0);
    }

    async::Future<std::error_code, Data> Stream::read(int min, int max)
    {
        assert(0);
    }

    std::error_code Stream::write(const Data &data)
    {
        assert(0);
    }

    void Stream::close()
    {
        switch(_engineType)
        {
        case EngineType::null:
            return;
        case EngineType::tcp:
            _engine._tcp->close();
            delete _engine._tcp;
            _engineType = EngineType::null;
            return;
        }

        assert(!"never here");
        abort();
    }

    bool Stream::isReadable()
    {
        assert(0);
    }

    bool Stream::isWriteable()
    {
        assert(0);
    }

}}}
