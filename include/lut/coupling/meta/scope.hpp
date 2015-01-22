#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/compound.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Scope;
    }

    class Scope
        : public hiddenImpl::Layout<impl::Scope, Type, Compound<Type>>
    {

    };

}}}
