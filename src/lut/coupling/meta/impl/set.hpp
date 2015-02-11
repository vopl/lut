#pragma once

#include "lut/coupling/meta/impl/container1.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Set
        : public himpl::ImplLayout<Set, Container1>
    {
    public:
        Set();
        ~Set();
    };

}}}}
