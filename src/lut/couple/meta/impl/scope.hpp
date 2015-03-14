#pragma once

#include "lut/couple/meta/impl/named.hpp"
#include "lut/couple/meta/impl/compound.hpp"
#include "lut/couple/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
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