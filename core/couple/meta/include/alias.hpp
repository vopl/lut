#pragma once

#include "type.hpp"
#include "named.hpp"

namespace dci { namespace couple { namespace meta
{
    class Alias
        : public himpl::FaceLayout<impl::Alias, Type, Named>
    {
    public:
        Type *target();

        bool strong();

    };

}}}
