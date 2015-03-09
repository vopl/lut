#pragma once

#include "lut/couple/meta/type.hpp"
#include "lut/couple/meta/scope.hpp"
#include "lut/couple/meta/compound.hpp"
#include "lut/couple/meta/attribute.hpp"
#include "lut/couple/meta/inheritable.hpp"

namespace lut { namespace couple { namespace meta
{
    class Struct
        : public himpl::FaceLayout<impl::Struct, Type, Scope, Compound<Attribute>, Inheritable<Struct>>
    {

    };

}}}
