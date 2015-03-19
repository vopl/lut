#pragma once

#include "named.hpp"
#include "type.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Attribute
        : public himpl::ImplLayout<Attribute, Named>
    {
    public:
        Attribute();
        ~Attribute();

        void setValueType(Type *v);

    private:
        Type *_valueType{nullptr};
    };

}}}}
