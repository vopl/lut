#pragma once

#include "lut/hiddenImpl/sizeofImpl.hpp"

namespace lut { namespace hiddenImpl
{
    template <class T>
    struct sizeofImpl
    {
        static const std::size_t _value = 0;
    };

}}
