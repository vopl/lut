#pragma once

#include "lut/couple/meta/type.hpp"
#include "lut/couple/meta/scope.hpp"
#include "lut/couple/meta/compound.hpp"
#include "lut/couple/meta/method.hpp"
#include "lut/couple/meta/inheritable.hpp"

namespace lut { namespace couple { namespace meta
{
    class Iface
        : public himpl::FaceLayout<impl::Iface, Type, Scope, Compound<Method>, Inheritable<Iface>>
    {
    public:
        std::vector<Method *> methods();

        bool forward();

        Iface *opposite();

    };

}}}
