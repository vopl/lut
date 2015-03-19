#pragma once

#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    class Iface
        : public himpl::ImplLayout<Iface>
    {

    public:
        Iface();
        ~Iface();
    };

}}}}
