#include "segment.hpp"

namespace dci { namespace io { namespace impl { namespace data
{
    Segment::Segment(std::uint32_t size, std::uint32_t offset, Segment *next)
        : _size {size}
        , _offset {offset}
        , _next {next}
    {
        assert(_offset+_size <= _granula);
    }

}}}}
