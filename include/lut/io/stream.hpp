#pragma once

#include "lut/hiddenImpl.hpp"
#if !GENERATE_SIZEOFIMPL
#   include "lut/io/sizeofImpl.hpp"
#endif
#include "lut/io/data.hpp"
#include "lut/async/future.hpp"

#include <system_error>
#include <limits>

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

        async::Future<std::error_code, io::Data> read(std::size_t min=1, std::size_t max = std::numeric_limits<std::size_t>::max());
        async::Future<std::error_code> write(io::Data &&data);

        void close();
    };
}}
