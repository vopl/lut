#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Integer;
    }

    class Integer
        : public hiddenImpl::Layout<impl::Integer, Type>
    {

    };

}}}
