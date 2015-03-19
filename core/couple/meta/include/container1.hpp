#pragma once

#include "type.hpp"

namespace dci { namespace couple { namespace meta
{
    class Container1
        : public himpl::FaceLayout<impl::Container1, Type>
    {
    public:
        Type *elementType();
    };

}}}
