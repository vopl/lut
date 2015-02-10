#pragma once

#include "lut/coupling/meta/impl/container1.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Ptr
        : public himpl::ImplLayout<Container1>
    {
    public:
        Ptr();
        ~Ptr();
    };

}}}}
