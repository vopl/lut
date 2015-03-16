#pragma once

#include "lut/himpl/faceLayout.hpp"

#if !GENERATE_SIZEPROVIDER
#   include "lut/module/sizeProvider.hpp"
#endif

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

        const std::string &getDir() const;
        void setDir(const std::string &dir);
    };
}}
