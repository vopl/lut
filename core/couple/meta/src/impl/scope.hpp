#pragma once

#include "named.hpp"
#include "compound.hpp"
#include "type.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
