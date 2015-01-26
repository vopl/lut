#pragma once

#include "lut/hiddenImpl/faceLayout.hpp"

namespace lut { namespace hiddenImpl
{
    class Accessor
    {
    public:

        template <class TImpl, class... TBaseFaces>
        static const TImpl &access(const hiddenImpl::FaceLayout<TImpl, TBaseFaces...> &impl)
        {
            return impl.impl();
        }

        template <class TImpl, class... TBaseFaces>
        static TImpl &access(hiddenImpl::FaceLayout<TImpl, TBaseFaces...> &impl)
        {
            return impl.impl();
        }
    };
}}
