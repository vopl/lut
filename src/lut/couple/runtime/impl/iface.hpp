#pragma once

#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace runtime { namespace impl
{
    class Iface
        : public himpl::ImplLayout<Iface>
    {

    public:
        Iface();
        ~Iface();
    };

}}}}
