#include "lut/stable.hpp"
#include "lut/couple/meta/impl/array.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
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
