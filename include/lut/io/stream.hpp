#pragma once

#include "lut/himpl/faceLayout.hpp"
#if !GENERATE_SIZEPROVIDER
#   include "lut/io/sizeProvider.hpp"
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
        : public himpl::FaceLayout<impl::Stream>
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
