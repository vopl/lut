#pragma once

#include "lut/coupling/meta/impl/type.hpp"
#include "lut/hiddenImpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Primitive
        : public hiddenImpl::ImplLayout<Type>
    {
    };

}}}}
