#pragma once

#include "lut/hiddenImpl/layout.hpp"

namespace lut { namespace hiddenImpl
{
    class Accessor
    {
    public:

        template <class TImpl, class... TFaces>
        static const TImpl &access(const hiddenImpl::Layout<TImpl, TFaces...> &impl)
        {
            return impl.impl();
        }

        template <class TImpl, class... TFaces>
        static TImpl &access(hiddenImpl::Layout<TImpl, TFaces...> &impl)
        {
            return impl.impl();
        }
    };
}}
