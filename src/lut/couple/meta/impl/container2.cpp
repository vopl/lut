#include "lut/stable.hpp"
#include "lut/couple/meta/impl/container2.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    Container2::Container2()
    {

    }

    Container2::~Container2()
    {

    }

    void Container2::setElementType1(Type *v)
    {
        _elementType1 = v;
    }

    void Container2::setElementType2(Type *v)
    {
        _elementType2 = v;
    }

}}}}
