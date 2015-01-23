#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/compound.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Scope
        : public Type
        , public Compound<Type>
    {
    };

}}}}
