#pragma once

#include "container1.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Set
        : public himpl::ImplLayout<Set, Container1>
    {
    public:
        Set();
        ~Set();
    };

}}}}
