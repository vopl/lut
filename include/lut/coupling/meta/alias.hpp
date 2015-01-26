#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Alias
        : public hiddenImpl::FaceLayout<impl::Alias, Type>
    {
    public:
        Type *target();

        bool strong();

    };

}}}
