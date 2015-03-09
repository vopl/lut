#pragma once

#include "lut/couple/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Container1
        : public himpl::ImplLayout<Container1, Type>
    {
    public:
        Container1();
        ~Container1();

        void setElementType(Type *v);

    private:
        Type *_elementType{nullptr};
    };

}}}}
