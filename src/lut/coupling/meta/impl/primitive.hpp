#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Primitive
        : public himpl::ImplLayout<Type>
    {
    public:
        Primitive();
        ~Primitive();
    };

}}}}
