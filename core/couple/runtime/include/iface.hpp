#pragma once

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")

#include "identifier.hpp"
#include "intrusivePtr.hpp"
#include <dci/himpl/faceLayout.hpp>

namespace dci { namespace couple { namespace runtime
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
