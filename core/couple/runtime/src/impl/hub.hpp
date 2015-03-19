#pragma once

#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    class Hub
        : public himpl::ImplLayout<Hub>
    {

    public:
        Hub();
        ~Hub();
    };

}}}}
