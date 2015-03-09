#pragma once

#include "lut/couple/meta/impl/named.hpp"
#include "lut/couple/meta/impl/compound.hpp"
#include "lut/couple/meta/impl/attribute.hpp"
#include "lut/couple/meta/impl/type.hpp"
#include "lut/couple/meta/method.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Method
        : public himpl::ImplLayout<Method, Named, Compound<Attribute>>
    {
    public:
        using Compound<Attribute>::add;

        Method();
        ~Method();

        void setResultType(Type *v);

        void setDirection(CallDirection v);

        void setNowait(bool v);

    private:
        Type            *_resultType{nullptr};
        CallDirection   _direction{CallDirection::in};
        bool            _nowait{false};

    };

}}}}
