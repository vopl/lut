#pragma once

#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/named.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Alias
        : public himpl::FaceLayout<impl::Alias, Type, Named>
    {
    public:
        Type *target();

        bool strong();

    };

}}}
