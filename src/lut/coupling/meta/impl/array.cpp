#include "lut/stable.hpp"
#include "lut/coupling/meta/impl/array.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    Array::Array()
    {

    }

    Array::~Array()
    {

    }

    void Array::setSize(std::uint32_t v)
    {
        _size = v;
    }

}}}}
