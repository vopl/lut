#pragma once

#include "type.hpp"
#include "named.hpp"
#include "compound.hpp"
#include "inheritable.hpp"
#include "enumValue.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
