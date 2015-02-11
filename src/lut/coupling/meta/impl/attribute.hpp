#pragma once

#include "lut/coupling/meta/impl/named.hpp"
#include "lut/coupling/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
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
