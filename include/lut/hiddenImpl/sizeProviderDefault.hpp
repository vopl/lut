#pragma once

#include "lut/hiddenImpl/sizeProvider.hpp"

namespace lut { namespace hiddenImpl
{
    template <class T>
    struct sizeProvider
    {
        static const std::size_t _value = 0;
    };

}}
