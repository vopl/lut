#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/scope.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/inheritable.hpp"
#include "lut/coupling/meta/impl/attribute.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
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
