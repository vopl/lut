#pragma once

#include "lut/himpl/faceLayout.hpp"
#include "lut/module/sizeProvider.hpp"

#include <string>

namespace lut { namespace module
{
    namespace impl
    {
        class Place;
    }

    class Place
        : public himpl::FaceLayout<impl::Place>
    {

    public:
        Place(const std::string &dir);
        ~Place();

    };
}}
