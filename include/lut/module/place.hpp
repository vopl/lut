#pragma once

#include "lut/himpl/faceLayout.hpp"
#include "lut/module/sizeProvider.hpp"

namespace lut { namespace module
{
    namespace impl
    {
        class Place;
    }

    struct Place
        : public himpl::FaceLayout<impl::Place>
    {

    };
}}
