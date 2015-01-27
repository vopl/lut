#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/himpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Container1
        : public himpl::FaceLayout<impl::Container1, Type>
    {
    public:
        Type *elementType();
    };

}}}
