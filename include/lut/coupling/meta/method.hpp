#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/compound.hpp"

namespace lut { namespace coupling { namespace meta
{
    enum class CallDirection
    {
        in,
        out
    };

    class Method
        : public hiddenImpl::FaceLayout<impl::Method, Type, Compound<Attribute>>
    {
    public:
        Iface *iface();

        Type *resultType();

        CallDirection callDirection();

        bool nowait();

    };

}}}
