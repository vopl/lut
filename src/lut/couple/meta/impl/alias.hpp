#pragma once

#include "lut/couple/meta/impl/type.hpp"
#include "lut/couple/meta/impl/named.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class Alias
        : public himpl::ImplLayout<Alias, Type, Named>
    {
    public:
        Alias();
        ~Alias();

        void setTarget(Type *v);

    private:
        Type *_target{nullptr};
    };

}}}}
