#pragma once

#include "lut/couple/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Primitive
        : public himpl::ImplLayout<Primitive, Type>
    {
    public:
        Primitive();
        ~Primitive();
    };

}}}}
