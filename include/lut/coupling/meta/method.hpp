#pragma once

#include "lut/coupling/meta/named.hpp"
#include "lut/coupling/meta/compound.hpp"
#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
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
