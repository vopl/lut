#pragma once

#include "lut/couple/meta/impl/type.hpp"
#include "lut/couple/meta/impl/named.hpp"
#include "lut/couple/meta/impl/compound.hpp"
#include "lut/couple/meta/impl/inheritable.hpp"
#include "lut/couple/meta/impl/enumValue.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Enum
        : public himpl::ImplLayout<Enum, Type, Named, Compound<EnumValue>, Inheritable<Enum>>
    {
    public:
        Enum();
        ~Enum();

        using Compound<EnumValue>::add;
    };

}}}}
