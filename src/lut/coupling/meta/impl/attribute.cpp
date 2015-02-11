#include "lut/stable.hpp"
#include "lut/coupling/meta/impl/attribute.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
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
