#pragma once

#include "lut/coupling/meta/impl/named.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class EnumValue
        : public himpl::ImplLayout<EnumValue, Named>
    {
    public:
        EnumValue();
        ~EnumValue();
    };

}}}}
