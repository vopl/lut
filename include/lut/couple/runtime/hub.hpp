#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/couple/runtime/sizeProvider.hpp"
#endif
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace couple { namespace runtime
{
    namespace impl
    {
        class Hub;
    }

    class Hub
        : public himpl::FaceLayout<impl::Hub>
    {
    };

}}}
