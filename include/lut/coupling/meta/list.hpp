#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/compound.hpp"

#include "lut/coupling/meta/container1.hpp"

namespace lut { namespace coupling { namespace meta
{

    namespace impl
    {
        class List;
    }

    class List
        : public hiddenImpl::Compound<Container1, impl::List>
    {

    };

}}}
