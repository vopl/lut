#include "lut/stable.hpp"
#include "lut/io/sizeofImpl.hpp"
#include "lut/io/stream.hpp"
#include "lut/io/impl/stream.hpp"

namespace lut { namespace io
{
    Stream::Stream()
        : HiddenImpl<impl::Stream>()
    {

    }

    Stream::Stream(Stream &&from)
        : HiddenImpl<impl::Stream>(std::forward<HiddenImpl<impl::Stream>>(from))
    {
    }

    Stream::~Stream()
    {
    }

    async::Future<std::error_code> Stream::shutdown(bool read, bool write)
    {
        return impl().shutdown(read, write);
    }

    async::Future<std::error_code, Data> Stream::read(int min, int max)
    {
        return impl().read(min, max);
    }

    std::error_code Stream::write(const Data &data)
    {
        return impl().write(data);
    }

    void Stream::close()
    {
        return impl().close();
    }

    bool Stream::isReadable()
    {
        return impl().isReadable();
    }

    bool Stream::isWriteable()
    {
        return impl().isWriteable();
    }


}}
