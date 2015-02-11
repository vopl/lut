#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/coupling/meta/sizeProvider.hpp"
#endif
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Named
        : public himpl::FaceLayout<impl::Named>
    {
    public:
        //имя
        std::string name();

    };

}}}
