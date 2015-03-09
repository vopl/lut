#pragma once

#include "lut/couple/meta/impl/container1.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Ptr
        : public himpl::ImplLayout<Ptr, Container1>
    {
    public:
        Ptr();
        ~Ptr();
    };

}}}}
