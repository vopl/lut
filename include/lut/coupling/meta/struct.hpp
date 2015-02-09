#pragma once

#include "lut/coupling/meta/scope.hpp"
#include "lut/coupling/meta/compound.hpp"
#include "lut/coupling/meta/attribute.hpp"
#include "lut/coupling/meta/inheritable.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Struct
        : public himpl::FaceLayout<impl::Struct, Scope, Compound<Attribute>, Inheritable<Struct>>
    {

    };

}}}
