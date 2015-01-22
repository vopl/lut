#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

#include "lut/coupling/meta/container2.hpp"

namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Map;
    }

    class Map
        : public hiddenImpl::Layout<impl::Map, Container2>
    {

    };

}}}
