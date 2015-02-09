#pragma once

#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/compound.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Scope
        : public himpl::FaceLayout<impl::Scope, Type, Compound<Type>>
    {

    };

}}}
