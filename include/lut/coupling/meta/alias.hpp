#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Alias
        : public hiddenImpl::Layout<impl::Alias, Type>
    {
    public:
        Type *target();

        bool strong();

    };

}}}
