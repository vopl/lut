#pragma once

#include "type.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
