#include "lut/stable.hpp"
#include "lut/io/impl/stream.hpp"
#include "lut/io/impl/fd/stream.hpp"
#include "lut/io/error.hpp"

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
        case EngineType::fd:
            _engine._fd = from._engine._fd;
            from._engineType = EngineType::null;
            return;
        }

        assert(!"never here");
        abort();
    }

    Stream::~Stream()
    {
        close();
    }

    void Stream::setEngine(fd::Stream *tcpEngine)
    {
        close();

        _engine._fd = tcpEngine;
        _engineType = EngineType::fd;
    }

    async::Future<std::error_code, io::Data> Stream::read(std::size_t min, std::size_t max)
    {
        switch(_engineType)
        {
        case EngineType::null:
            {
                return async::mkReadyFuture(make_error_code(error::stream::not_connected), io::Data());
            }
        case EngineType::fd:
            return _engine._fd->read(min, max);
        }

        assert(!"never here");
        abort();
    }

    async::Future<std::error_code> Stream::write(io::Data &&data)
    {
        switch(_engineType)
        {
        case EngineType::null:
            {
                return async::mkReadyFuture(make_error_code(error::stream::not_connected));
            }
        case EngineType::fd:
            return _engine._fd->write(std::forward<io::Data>(data));
        }

        assert(!"never here");
        abort();
    }

    void Stream::close()
    {
        switch(_engineType)
        {
        case EngineType::null:
            return;
        case EngineType::fd:
            _engine._fd->fdClose();
            delete _engine._fd;
            _engineType = EngineType::null;
            return;
        }

        assert(!"never here");
        abort();
    }

}}}
