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

        async::Future<std::error_code> shutdown(bool read, bool write);

        async::Future<std::error_code, Data> read(int min, int max);
        std::error_code write(const Data &data);

        void close();

        bool isReadable();
        bool isWriteable();
    };
}}
