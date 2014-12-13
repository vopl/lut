#pragma once

#include "lut/hiddenImpl.hpp"
#include "lut/io/sizeofImpl.hpp"
#include "lut/io/data.hpp"
#include "lut/async/future.hpp"

#include <system_error>

namespace lut { namespace io
{
    namespace impl
    {
        class Stream;
    }

    class Stream
        : public HiddenImpl<impl::Stream>
    {
        Stream(const Stream &from) = delete;
        Stream &operator=(const Stream &from) = delete;


    public:
        Stream();
        Stream(Stream &&from);
        ~Stream();

        Stream &operator=(Stream &&from);

    public:

        async::Future<std::error_code, io::Data> read(std::size_t min, std::size_t max);
        async::Future<std::error_code> write(io::Data &&data);

        void close();
    };
}}
