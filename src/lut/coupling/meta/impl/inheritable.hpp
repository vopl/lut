#pragma once

#include "lut/hiddenImpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    template <class B>
    class Inheritable
        : public hiddenImpl::ImplLayout<>
    {
    };

}}}}
