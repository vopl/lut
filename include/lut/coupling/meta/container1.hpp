#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Container1
        : public hiddenImpl::Layout<impl::Container1, Type>
    {
    public:
        Type *elementType();
    };

}}}
