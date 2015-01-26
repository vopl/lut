#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/coupling/meta/impl/compound.hpp"
#include "lut/hiddenImpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Scope
        : public hiddenImpl::ImplLayout<Type, Compound<Type>>
    {
    };

}}}}
