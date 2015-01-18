#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/single.hpp"


namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Type;
    }

    class Type
        : public hiddenImpl::Single<impl::Type>
    {

    };

}}}
