#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        template <class B>
        class Inheritable;
    }

    template <class B>
    class Inheritable
        : public hiddenImpl::Layout<impl::Inheritable<B>>
    {

    };

}}}
