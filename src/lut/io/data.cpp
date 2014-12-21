#include "lut/stable.hpp"
#include "lut/io/sizeofImpl.hpp"
#include "lut/io/data.hpp"
#include "lut/io/impl/data.hpp"

namespace lut { namespace io
{
    Data::Data()
        : HiddenImpl<impl::Data>()
    {
    }

    Data::Data(Data &&from)
        : HiddenImpl<impl::Data>(std::forward<HiddenImpl<impl::Data>>(from))
    {
    }

    Data::Data(impl::Data &&from)
        : HiddenImpl<impl::Data>(std::forward<impl::Data>(from))
    {
    }

    Data::~Data()
    {
    }

    Data &Data::operator=(Data &&from)
    {
        this->HiddenImpl<impl::Data>::operator =(std::forward<HiddenImpl<impl::Data>>(from));
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

}}
