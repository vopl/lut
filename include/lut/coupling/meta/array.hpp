#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/container1.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Array;
    }

    class Array
        : public hiddenImpl::Layout<impl::Array, Container1>
    {

    };

}}}
