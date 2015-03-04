#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/coupling/runtime/sizeProvider.hpp"
#endif
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace coupling { namespace runtime
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
