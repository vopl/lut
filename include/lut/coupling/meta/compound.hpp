#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/single.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        template <class E>
        class Compound;
    }

    template <class E>
    class Compound
        : public hiddenImpl::Single<impl::Compound<E>>
    {

    };

}}}
