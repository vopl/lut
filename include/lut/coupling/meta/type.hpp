#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/entity.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Type;
    }

    class Type
        : public hiddenImpl::Layout<impl::Type, Entity>
    {

    };

}}}
