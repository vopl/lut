#pragma once

#include "lut/coupling/meta/impl/container1.hpp"
#include "lut/coupling/meta/impl/container2.hpp"
#include "lut/hiddenImpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Array
        : public hiddenImpl::ImplLayout<Container1>
    {
    };

}}}}
