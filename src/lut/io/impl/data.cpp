#include "lut/stable.hpp"
#include "lut/io/impl/data.hpp"

namespace lut { namespace io { namespace impl
{
    Data::Data()
    {
        assert(0);
    }

    Data::Data(Data &&)
    {
        assert(0);
    }

    Data::~Data()
    {
        assert(0);
    }

    Data &Data::operator=(Data &&)
    {
        assert(0);
    }

    void Data::append(Data &&data)
    {
        assert(0);
    }

    void Data::append(const char *str, std::size_t size)
    {
        assert(0);
    }

    bool Data::empty() const
    {
        assert(0);
    }

    std::size_t Data::size() const
    {
        assert(0);
    }

    void Data::clear()
    {
        assert(0);
    }

    std::size_t Data::segmentsAmount() const
    {
        assert(0);
    }

    void Data::fillIovec(iovec *iov) const
    {
        assert(0);
    }

    void Data::dropFirst(std::size_t size)
    {
        assert(0);
    }

    void Data::dropLast(std::size_t size)
    {
        assert(0);
    }


}}}
