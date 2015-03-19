#pragma once

#include "container1.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Ptr
        : public himpl::ImplLayout<Ptr, Container1>
    {
    public:
        Ptr();
        ~Ptr();
    };

}}}}
