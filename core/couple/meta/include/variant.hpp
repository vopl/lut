#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "attribute.hpp"
#include "inheritable.hpp"

namespace dci { namespace couple { namespace meta
{
    class Variant
        : public himpl::FaceLayout<impl::Variant, Type, Scope, Compound<Attribute>, Inheritable<Variant>>
    {

    };

}}}
