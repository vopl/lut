#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Container1
        : public himpl::ImplLayout<Type>
    {
    public:
        Container1();
        ~Container1();

        void setElementType(Type *v);
    };

}}}}
