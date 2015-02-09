#pragma once

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Attribute
        : public himpl::FaceLayout<impl::Attribute, Type>
    {
    public:
        Type *valueType();

    };

}}}
