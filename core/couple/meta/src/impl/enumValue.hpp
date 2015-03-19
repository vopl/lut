#pragma once

#include "named.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class EnumValue
        : public himpl::ImplLayout<EnumValue, Named>
    {
    public:
        EnumValue();
        ~EnumValue();
    };

}}}}
