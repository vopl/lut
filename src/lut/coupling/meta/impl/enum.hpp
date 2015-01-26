#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/coupling/meta/impl/inheritable.hpp"
#include "lut/coupling/meta/impl/enumValue.hpp"
#include "lut/hiddenImpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Enum
        : public hiddenImpl::ImplLayout<Type, Compound<EnumValue>, Inheritable<Enum>>
    {
    };

}}}}
