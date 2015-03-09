#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/couple/runtime/sizeProvider.hpp"
#endif
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace couple { namespace runtime
{
    namespace impl
    {
        class Iface;
    }

    class Iface
        : public himpl::FaceLayout<impl::Iface>
    {
    };

}}}
