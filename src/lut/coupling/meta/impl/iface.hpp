#pragma once

#include "lut/coupling/meta/impl/scope.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/inheritable.hpp"
#include "lut/coupling/meta/impl/method.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Iface
        : public Scope
        , public Compound<Method>
        , public Inheritable<Iface>
    {

    };

}}}}
