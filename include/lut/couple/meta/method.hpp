#pragma once

#include "lut/couple/meta/named.hpp"
#include "lut/couple/meta/compound.hpp"
#include "lut/couple/meta/type.hpp"

namespace lut { namespace couple { namespace meta
{
    enum class CallDirection
    {
        in,
        out
    };

    class Method
        : public himpl::FaceLayout<impl::Method, Named, Compound<Attribute>>
    {
    public:
        Iface *iface();

        Type *resultType();

        CallDirection direction();

        bool nowait();

    };

}}}
