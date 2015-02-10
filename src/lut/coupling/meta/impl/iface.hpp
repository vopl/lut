#pragma once

#include "lut/coupling/meta/impl/scope.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/inheritable.hpp"
#include "lut/coupling/meta/impl/method.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Iface
        : public himpl::ImplLayout<Scope, Compound<Method>, Inheritable<Iface>>
    {
    public:
        Iface();
        ~Iface();

        using Compound<Method>::add;
        using Compound<Type>::add;

    };

}}}}
