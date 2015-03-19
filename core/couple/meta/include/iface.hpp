#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "method.hpp"
#include "inheritable.hpp"

namespace dci { namespace couple { namespace meta
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
