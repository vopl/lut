#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/scope.hpp"
#include "lut/coupling/meta/compound.hpp"
#include "lut/coupling/meta/attribute.hpp"
#include "lut/coupling/meta/inheritable.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Variant;
    }

    class Variant
        : public hiddenImpl::Layout<impl::Variant, Scope, Compound<Attribute>, Inheritable<Variant>>
    {

    };

}}}
