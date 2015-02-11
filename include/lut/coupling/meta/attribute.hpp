#pragma once

#include "lut/coupling/meta/named.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Attribute
        : public himpl::FaceLayout<impl::Attribute, Named>
    {
    public:
        Type *valueType();

    };

}}}
