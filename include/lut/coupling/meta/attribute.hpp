#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/compound.hpp"

#include "lut/coupling/meta/entity.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Attribute;
    }

    class Attribute
        : public hiddenImpl::Compound<Entity, impl::Attribute>
    {

    };

}}}
