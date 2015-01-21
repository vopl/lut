#pragma once

#include "lut/coupling/meta/impl/scope.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/inheritable.hpp"
#include "lut/coupling/meta/impl/attribute.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Variant
        : public Scope
        , public Compound<Attribute>
        , public Inheritable<Variant>
    {

    };

}}}}
