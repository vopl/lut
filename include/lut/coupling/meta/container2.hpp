#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Container2;
    }

    class Container2
        : public hiddenImpl::Layout<impl::Container2, Type>
    {

    };

}}}
