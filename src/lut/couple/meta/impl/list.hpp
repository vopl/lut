#pragma once

#include "lut/couple/meta/impl/container1.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class List
        : public himpl::ImplLayout<List, Container1>
    {
    public:
        List();
        ~List();
    };

}}}}
