#include "lut/stable.hpp"
#include "lut/io/sizeProvider.hpp"
#include "lut/io/stream.hpp"
#include "lut/io/impl/stream.hpp"

namespace lut { namespace io
{
    Stream::Stream()
        : hiddenImpl::Layout<impl::Stream>()
    {

    }

    Stream::Stream(Stream &&from)
        : hiddenImpl::Layout<impl::Stream>(std::forward<hiddenImpl::Layout<impl::Stream>>(from))
    {
    }

    Stream::~Stream()
    {
    }

    async::Future<std::error_code, io::Data> Stream::read(std::size_t min, std::size_t max)
    {
        return impl().read(min, max);
    }

    async::Future<std::error_code> Stream::write(Data &&data)
    {
        return impl().write(std::forward<Data>(data));
    }

    void Stream::close()
    {
        return impl().close();
    }

}}
