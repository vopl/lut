#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Attribute
        : public hiddenImpl::Layout<impl::Attribute, Type>
    {
    public:
        Type *valueType();

    };

}}}
