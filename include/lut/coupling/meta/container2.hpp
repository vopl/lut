#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/compound.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Container2;
    }

    class Container2
        : public hiddenImpl::Compound<Type, impl::Container2>
    {

    };

}}}
