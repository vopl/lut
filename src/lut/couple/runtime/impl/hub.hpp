#pragma once

#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace runtime { namespace impl
{
    class Hub
        : public himpl::ImplLayout<Hub>
    {

    public:
        Hub();
        ~Hub();
    };

}}}}
