#pragma once

#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace runtime { namespace impl
{
    class Hub
        : public himpl::ImplLayout<Hub>
    {

    public:
        Hub();
        ~Hub();
    };

}}}}
