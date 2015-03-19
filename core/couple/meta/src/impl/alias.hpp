#pragma once

#include "type.hpp"
#include "named.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
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
