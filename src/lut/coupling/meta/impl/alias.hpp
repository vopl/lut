#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/named.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
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
