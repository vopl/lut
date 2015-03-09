#pragma once

#include "lut/couple/meta/type.hpp"

namespace lut { namespace couple { namespace meta
{
    class Container1
        : public himpl::FaceLayout<impl::Container1, Type>
    {
    public:
        Type *elementType();
    };

}}}
