#pragma once

#include "named.hpp"
#include "compound.hpp"
#include "attribute.hpp"
#include "type.hpp"
#include <method.hpp>
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
