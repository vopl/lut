#pragma once

#include "type.hpp"

namespace dci { namespace couple { namespace meta
{
    class Container2
        : public himpl::FaceLayout<impl::Container2, Type>
    {
    public:
        Type *elementType1();
        Type *elementType2();
    };

}}}
