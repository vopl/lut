#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "attribute.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Variant
        : public himpl::ImplLayout<Variant, Type, Scope, Compound<Attribute>, Inheritable<Variant>>
    {
    public:
        using Compound<Attribute>::add;
        using Compound<Type>::add;

        Variant();
        ~Variant();
    };

}}}}
