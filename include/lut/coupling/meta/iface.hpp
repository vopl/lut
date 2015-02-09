#pragma once

#include "lut/coupling/meta/scope.hpp"
#include "lut/coupling/meta/compound.hpp"
#include "lut/coupling/meta/method.hpp"
#include "lut/coupling/meta/inheritable.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Iface
        : public himpl::FaceLayout<impl::Iface, Scope, Compound<Method>, Inheritable<Iface>>
    {
    public:
        std::vector<Method *> methods();

        bool forward();

        Iface *opposite();

    };

}}}
