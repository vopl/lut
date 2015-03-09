#pragma once

#include "lut/couple/meta/named.hpp"
#include "lut/couple/meta/type.hpp"
#include "lut/couple/meta/compound.hpp"

namespace lut { namespace couple { namespace meta
{
    class Scope
        : public himpl::FaceLayout<impl::Scope, Named, Compound<Type>, Compound<Scope>>
    {

    };

}}}
