#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Container1
        : public hiddenImpl::FaceLayout<impl::Container1, Type>
    {
    public:
        Type *elementType();
    };

}}}
