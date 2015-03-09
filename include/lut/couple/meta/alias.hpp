#pragma once

#include "lut/couple/meta/type.hpp"
#include "lut/couple/meta/named.hpp"

namespace lut { namespace couple { namespace meta
{
    class Alias
        : public himpl::FaceLayout<impl::Alias, Type, Named>
    {
    public:
        Type *target();

        bool strong();

    };

}}}
