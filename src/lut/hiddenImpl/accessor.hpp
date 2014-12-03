#pragma once

#include "lut/hiddenImpl.hpp"

namespace lut { namespace hiddenImpl
{
    class Accessor
    {
    public:

        template <typename T>
        static const T &access(const HiddenImpl<T> &impl)
        {
            return impl.impl();
        }

        template <typename T>
        static T &access(HiddenImpl<T> &impl)
        {
            return impl.impl();
        }
    };
}}
