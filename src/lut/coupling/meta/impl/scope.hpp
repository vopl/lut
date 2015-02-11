#pragma once

#include "lut/coupling/meta/impl/named.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Scope
        : public himpl::ImplLayout<Scope, Named, Compound<Type>, Compound<Scope>>
    {
    public:
        using Compound<Type>::add;
        using Compound<Scope>::add;

        Scope();
        ~Scope();
    };

}}}}
