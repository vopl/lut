#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Attribute
        : public hiddenImpl::FaceLayout<impl::Attribute, Type>
    {
    public:
        Type *valueType();

    };

}}}
