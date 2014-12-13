#include "lut/io/impl/stream.hpp"
#include "lut/io/impl/fd/stream.hpp"

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

        _engine._tcp = tcpEngine;
        _engineType = EngineType::tcp;
    }

    async::Future<std::error_code, io::Data> Stream::read(std::size_t min, std::size_t max)
    {
        switch(_engineType)
        {
        case EngineType::null:
            {
                async::Promise<std::error_code, io::Data> promise;
                promise.setValue(std::make_error_code(std::errc::not_connected), io::Data());
                return promise.future();
            }
        case EngineType::tcp:
            return _engine._tcp->read(min, max);
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
                async::Promise<std::error_code> promise;
                promise.setValue(std::make_error_code(std::errc::not_connected));
                return promise.future();
            }
        case EngineType::tcp:
            return _engine._tcp->write(std::forward<io::Data>(data));
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
        case EngineType::tcp:
            _engine._tcp->close();
            delete _engine._tcp;
            _engineType = EngineType::null;
            return;
        }

        assert(!"never here");
        abort();
    }

}}}
