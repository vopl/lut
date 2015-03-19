#pragma once

#include "type.hpp"
#include "scope.hpp"
#include "compound.hpp"
#include "attribute.hpp"
#include "inheritable.hpp"

namespace dci { namespace couple { namespace meta
{
    class Struct
        : public himpl::FaceLayout<impl::Struct, Type, Scope, Compound<Attribute>, Inheritable<Struct>>
    {

    };

}}}
