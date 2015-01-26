#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Container2
        : public hiddenImpl::FaceLayout<impl::Container2, Type>
    {
    public:
        Type *elementType1();
        Type *elementType2();
    };

}}}
