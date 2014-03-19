#ifndef _LUT_MM_IMPL_SIZEDBUFFERCALCULATOR_HPP_
#define _LUT_MM_IMPL_SIZEDBUFFERCALCULATOR_HPP_

#include <cstdint>

namespace lut { namespace mm { namespace impl
{
    template <std::size_t value>
    struct SizedBufferCalculator
    {
        static const std::size_t _faceSize2ImplSize =
                value <= 8 ?
                    8 :
                    ((value % 16) ? (value - value%16 + 16) : (value));

        static const std::size_t _implAmount =
                value/16+1;

        static const std::size_t _implIndex2ImplSize =
                value == 0 ?
                    8 :
                    value * 16;

        static const std::size_t _implSize2ImplIndex =
                value/16;
    };

}}}

#endif
