#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/compound.hpp"

#include "lut/coupling/meta/scope.hpp"
#include "lut/coupling/meta/compound.hpp"
#include "lut/coupling/meta/method.hpp"
#include "lut/coupling/meta/inheritable.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Iface;
    }

    class Iface
        : public hiddenImpl::Compound3<Scope, Compound<Method>, Inheritable<Iface>, impl::Iface>
    {

    };

}}}
