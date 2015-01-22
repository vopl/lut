#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/entity.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Method;
    }

    class Method
        : public hiddenImpl::Layout<impl::Method, Entity>
    {

    };

}}}
