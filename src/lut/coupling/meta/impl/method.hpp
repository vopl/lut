#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/attribute.hpp"
#include "lut/coupling/meta/method.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Method
        : public himpl::ImplLayout<Type, Compound<Attribute>>
    {
    public:
        using Compound<Attribute>::add;

        Method();
        ~Method();

        void setResultType(Type *v);

        void setDirection(CallDirection v);

        void setNowait(bool v);

    };

}}}}
