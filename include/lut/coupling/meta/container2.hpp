#pragma once

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Container2
        : public himpl::FaceLayout<impl::Container2, Type>
    {
    public:
        Type *elementType1();
        Type *elementType2();
    };

}}}
