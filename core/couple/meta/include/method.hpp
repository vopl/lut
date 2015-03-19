#pragma once

#include "named.hpp"
#include "compound.hpp"
#include "type.hpp"

namespace dci { namespace couple { namespace meta
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
