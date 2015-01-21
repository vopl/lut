#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/compound.hpp"

#include "lut/coupling/meta/container2.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Map;
    }

    class Map
        : public hiddenImpl::Compound<Container2, impl::Map>
    {

    };

}}}
