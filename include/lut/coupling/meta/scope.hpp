#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/compound.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Scope
        : public hiddenImpl::FaceLayout<impl::Scope, Type, Compound<Type>>
    {

    };

}}}
