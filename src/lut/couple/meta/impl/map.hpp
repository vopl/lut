#pragma once

#include "lut/couple/meta/impl/container2.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Map
        : public himpl::ImplLayout<Map, Container2>
    {
    public:
        Map();
        ~Map();
    };

}}}}
