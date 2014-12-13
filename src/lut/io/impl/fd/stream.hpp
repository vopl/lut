#pragma once

#include "lut/io/impl/fd/base.hpp"
#include "lut/io/impl/fd/stream/reader.hpp"
#include "lut/io/impl/fd/stream/writer.hpp"


namespace lut { namespace io { namespace impl { namespace fd
{

    class Stream
        : public Base
    {
    public:
        Stream();
        ~Stream() override;

        void event(int typeFlags) override;

        async::Future<std::error_code, io::Data> read(std::size_t min, std::size_t max);
        async::Future<std::error_code> write(io::Data &&data);

        void close() override;

    private:
        stream::Reader _reader;
        stream::Writer _writer;
    };

}}}}
