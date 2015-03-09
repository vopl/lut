#pragma once

#include "lut/couple/meta/named.hpp"

namespace lut { namespace couple { namespace meta
{
    class Attribute
        : public himpl::FaceLayout<impl::Attribute, Named>
    {
    public:
        Type *valueType();

    };

}}}
