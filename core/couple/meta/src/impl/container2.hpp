#pragma once

#include "type.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
