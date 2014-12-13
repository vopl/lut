#include "lut/stable.hpp"
#include "lut/io/impl/fd/stream/reader.hpp"

namespace lut { namespace io { namespace impl { namespace fd { namespace stream
{

    Reader::Reader()
    {
    }

    Reader::~Reader()
    {
    }

    void Reader::error(const std::error_code &err)
    {
        assert(0);
    }

    void Reader::ready(int descriptor)
    {
        assert(0);
    }

    async::Future<std::error_code, io::Data> Reader::read(int descriptor, std::size_t min, std::size_t max)
    {
        assert(0);
    }

    void Reader::close()
    {
        assert(0);
    }

}}}}}
