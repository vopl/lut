#pragma once

#include "lut/couple/meta/impl/type.hpp"
#include "lut/couple/meta/impl/scope.hpp"
#include "lut/couple/meta/impl/compound.hpp"
#include "lut/couple/meta/impl/inheritable.hpp"
#include "lut/couple/meta/impl/method.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Iface
        : public himpl::ImplLayout<Iface, Type, Scope, Compound<Method>, Inheritable<Iface>>
    {
    public:
        Iface();
        ~Iface();

        using Compound<Method>::add;
        using Compound<Type>::add;

    };

}}}}
