#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "method.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
