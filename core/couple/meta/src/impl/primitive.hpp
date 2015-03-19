#pragma once

#include "type.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Primitive
        : public himpl::ImplLayout<Primitive, Type>
    {
    public:
        Primitive();
        ~Primitive();
    };

}}}}
