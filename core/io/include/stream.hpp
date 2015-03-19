#pragma once

#include <dci/himpl/faceLayout.hpp>
#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")
#include "data.hpp"
#include <dci/async/future.hpp>

#include <system_error>
#include <limits>

namespace dci { namespace io
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
