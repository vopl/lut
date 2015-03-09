#include "lut/stable.hpp"
#include "lut/couple/meta/impl/attribute.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    Attribute::Attribute()
    {

    }

    Attribute::~Attribute()
    {

    }

    void Attribute::setValueType(Type *v)
    {
        _valueType = v;
    }

}}}}
