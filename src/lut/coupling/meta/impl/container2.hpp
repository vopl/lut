#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Container2
        : public himpl::ImplLayout<Type>
    {
    public:
        Container2();
        ~Container2();

        void setElementType1(Type *v);
        void setElementType2(Type *v);

    };

}}}}
