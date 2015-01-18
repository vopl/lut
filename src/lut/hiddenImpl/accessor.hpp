#pragma once

#include "lut/hiddenImpl/single.hpp"

namespace lut { namespace hiddenImpl
{
    class Accessor
    {
    public:

        template <typename T>
        static const T &access(const Single<T> &impl)
        {
            return impl.impl();
        }

        template <typename T>
        static T &access(Single<T> &impl)
        {
            return impl.impl();
        }
    };
}}
