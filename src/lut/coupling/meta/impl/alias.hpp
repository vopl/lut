#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Alias
        : public himpl::ImplLayout<Type>
    {
    public:
        Alias();
        ~Alias();

        void setTarget(Type *v);
    };

}}}}
