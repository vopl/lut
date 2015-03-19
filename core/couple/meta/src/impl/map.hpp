#pragma once

#include "container2.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Map
        : public himpl::ImplLayout<Map, Container2>
    {
    public:
        Map();
        ~Map();
    };

}}}}
