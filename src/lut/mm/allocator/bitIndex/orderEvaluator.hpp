#pragma once

#include "lut/mm/allocator/bitIndex/level.hpp"

namespace lut { namespace mm { namespace allocator { namespace bitIndex
{
    template <std::size_t volume, std::size_t base=0>
    struct OrderEvaluator
    {
        struct Current
        {
            static constexpr std::size_t _order = base;
        };

        static constexpr std::size_t _order = std::conditional<volume <= Level<base>::_volume,
            Current,
            OrderEvaluator<volume, base+1> >::type::_order;
    };

}}}}
