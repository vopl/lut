#pragma once

#include "lut/couple/meta/impl/named.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class EnumValue
        : public himpl::ImplLayout<EnumValue, Named>
    {
    public:
        EnumValue();
        ~EnumValue();
    };

}}}}
