#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/named.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/inheritable.hpp"
#include "lut/coupling/meta/impl/enumValue.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
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
