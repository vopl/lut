#include "lut/stable.hpp"
#include "lut/io/sizeProvider.hpp"
#include "lut/io/data.hpp"
#include "lut/io/impl/data.hpp"

namespace lut { namespace io
{
    Data::Data()
        : himpl::FaceLayout<impl::Data>()
    {
    }

    Data::Data(Data &&from)
        : himpl::FaceLayout<impl::Data>(std::forward<himpl::FaceLayout<impl::Data>>(from))
    {
    }

    Data::Data(impl::Data &&from)
        : himpl::FaceLayout<impl::Data>(std::forward<impl::Data>(from))
    {
    }

    Data::~Data()
    {
    }

    Data &Data::operator=(Data &&from)
    {
        impl() = std::move(from.impl());
        return *this;
    }

    bool Data::empty() const
    {
        return impl().empty();
    }

    std::size_t Data::size() const
    {
        return impl().size();
    }

    void Data::append(Data &&data)
    {
        return impl().append(std::move(data.impl()));
    }

    void Data::append(const char *str, std::size_t size)
    {
        return impl().append(str, size);
    }

    void Data::clear()
    {
        return impl().clear();
    }

    Data &Data::operator<<(const char *str)
    {
        impl().append(str, strlen(str));
        return *this;
    }

    std::size_t Data::segmentsAmount() const
    {
        return impl().segmentsAmount();
    }

    void Data::fillIovec(iovec *iov) const
    {
        return impl().fillIovec(iov);
    }

    void Data::dropFirst(std::size_t size)
    {
        return impl().dropFirst(size);
    }

    void Data::dropLast(std::size_t size)
    {
        return impl().dropLast(size);
    }

    Data Data::detachFirst(std::size_t size)
    {
        return impl().detachFirst(size);
    }

    Data Data::detachLast(std::size_t size)
    {
        return impl().detachLast(size);
    }

    std::string Data::toString()
    {
        return impl().toString();
    }

}}
