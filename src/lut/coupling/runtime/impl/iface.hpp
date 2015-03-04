#pragma once

#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace runtime { namespace impl
{
    class Iface
        : public himpl::ImplLayout<Iface>
    {

    public:
        Iface();
        ~Iface();
    };

}}}}
