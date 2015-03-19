#pragma once

#include "named.hpp"

namespace dci { namespace couple { namespace meta
{
    class Attribute
        : public himpl::FaceLayout<impl::Attribute, Named>
    {
    public:
        Type *valueType();

    };

}}}
