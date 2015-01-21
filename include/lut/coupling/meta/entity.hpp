#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/single.hpp"


namespace lut { namespace coupling { namespace meta
{
    namespace impl
    {
        class Entity;
    }

    class Entity
        : public hiddenImpl::Single<impl::Entity>
    {

    };

}}}
