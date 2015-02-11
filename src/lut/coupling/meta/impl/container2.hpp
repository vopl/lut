#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Container2
        : public himpl::ImplLayout<Container2, Type>
    {
    public:
        Container2();
        ~Container2();

        void setElementType1(Type *v);
        void setElementType2(Type *v);

    private:
        Type *_elementType1{nullptr};
        Type *_elementType2{nullptr};
    };

}}}}
