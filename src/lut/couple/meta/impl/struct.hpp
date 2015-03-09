#pragma once

#include "lut/couple/meta/impl/type.hpp"
#include "lut/couple/meta/impl/scope.hpp"
#include "lut/couple/meta/impl/compound.hpp"
#include "lut/couple/meta/impl/inheritable.hpp"
#include "lut/couple/meta/impl/attribute.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Struct
        : public himpl::ImplLayout<Struct, Type, Scope, Compound<Attribute>, Inheritable<Struct>>
    {
    public:
        using Compound<Attribute>::add;
        using Compound<Type>::add;

        Struct();
        ~Struct();
    };

}}}}
