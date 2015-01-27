#pragma once

#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace himpl
{
    class Accessor
    {
    public:

        template <class TImpl, class... TBaseFaces>
        static const TImpl &access(const himpl::FaceLayout<TImpl, TBaseFaces...> &impl)
        {
            return impl.impl();
        }

        template <class TImpl, class... TBaseFaces>
        static TImpl &access(himpl::FaceLayout<TImpl, TBaseFaces...> &impl)
        {
            return impl.impl();
        }
    };
}}
