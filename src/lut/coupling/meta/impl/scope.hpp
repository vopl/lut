#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Scope
        : public himpl::ImplLayout<Type, Compound<Type>>
    {
    public:
        using Compound<Type>::add;

        Scope();
        ~Scope();
    };

}}}}
