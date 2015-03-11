#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/couple/runtime/sizeProvider.hpp"
#endif
#include "lut/couple/runtime/identifier.hpp"
#include "lut/couple/runtime/intrusivePtr.hpp"
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace couple { namespace runtime
{
    namespace impl
    {
        class Iface;
    }

    class IfaceIdentifierTag;
    using Iid = Identifier<IfaceIdentifierTag>;

    class Iface
        : public himpl::FaceLayout<impl::Iface>
    {
    };

    using IfacePtr = IntrusivePtr<Iface>;

}}}
