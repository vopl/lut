#pragma once

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Alias
        : public himpl::FaceLayout<impl::Alias, Type>
    {
    public:
        Type *target();

        bool strong();

    };

}}}
