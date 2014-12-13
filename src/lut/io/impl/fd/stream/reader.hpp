#pragma once

#include "lut/async/future.hpp"
#include "lut/io/data.hpp"

#include <system_error>

namespace lut { namespace io { namespace impl { namespace fd { namespace stream
{

    class Reader
    {
    public:
        Reader();
        ~Reader();

        void error(const std::error_code &err);
        void ready(int descriptor);
        async::Future<std::error_code, io::Data> read(int descriptor, std::size_t min, std::size_t max);
        void close();

    };

}}}}}
