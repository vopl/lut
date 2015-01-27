#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/himpl/faceLayout.hpp"

#include "lut/coupling/meta/container1.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Ptr
        : public himpl::FaceLayout<impl::Ptr, Container1>
    {

    };

}}}
