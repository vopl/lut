#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/himpl/faceLayout.hpp"

#include "lut/coupling/meta/container2.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Map
        : public himpl::FaceLayout<impl::Map, Container2>
    {

    };

}}}