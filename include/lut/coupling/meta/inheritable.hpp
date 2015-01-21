#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/single.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        template <class B>
        class Inheritable;
    }

    template <class B>
    class Inheritable
        : public hiddenImpl::Single<impl::Inheritable<B>>
    {

    };

}}}
