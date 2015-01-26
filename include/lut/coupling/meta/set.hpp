#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/container1.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Set
        : public hiddenImpl::FaceLayout<impl::Set, Container1>
    {

    };

}}}
