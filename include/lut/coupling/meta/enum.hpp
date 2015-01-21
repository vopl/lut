#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/compound.hpp"

#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/compound.hpp"
#include "lut/coupling/meta/enumValue.hpp"
#include "lut/coupling/meta/inheritable.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Enum;
    }

    class Enum
        : public hiddenImpl::Compound<Type, impl::Enum>
        , public Compound<EnumValue>
        , public Inheritable<Enum>
    {

    };

}}}
