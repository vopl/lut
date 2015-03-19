#pragma once

#include "sized_cast.hpp"

namespace dci { namespace mm { namespace utils
{
    template <typename To, typename From>
    To sized_cast(From from)
    {
        static_assert(sizeof(To) == sizeof(From), "operands must be same size for sized_cast");
        return (To)(from);
    }
}}}
