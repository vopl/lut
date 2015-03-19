#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "attribute.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
