#pragma once

#include "container1.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class List
        : public himpl::ImplLayout<List, Container1>
    {
    public:
        List();
        ~List();
    };

}}}}
