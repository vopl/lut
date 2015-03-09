#pragma once

#include "lut/couple/meta/impl/named.hpp"
#include "lut/couple/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
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
