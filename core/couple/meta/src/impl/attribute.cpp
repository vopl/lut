#include "attribute.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
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
