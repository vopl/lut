#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE(<sizeProvider.hpp>)
#include <stream.hpp>
#include "impl/stream.hpp"

namespace dci { namespace io
{
    Stream::Stream()
        : himpl::FaceLayout<impl::Stream>()
    {

    }

    Stream::Stream(Stream &&from)
        : himpl::FaceLayout<impl::Stream>(std::forward<himpl::FaceLayout<impl::Stream>>(from))
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
