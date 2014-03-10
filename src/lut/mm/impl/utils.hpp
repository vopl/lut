#ifndef _LUT_ASYNC_IMPL_SM_UTILS_HPP_
#define _LUT_ASYNC_IMPL_SM_UTILS_HPP_

namespace lut { namespace mm { namespace impl { namespace utils
{
    template<size_t bittness>
    struct IntegralSelector
    {

        using type =
            typename std::conditional<bittness <= 8, u_int8_t,
                typename std::conditional<bittness <= 16, u_int16_t,
                    typename std::conditional<bittness <= 32, u_int32_t,
                        typename std::conditional<bittness <= 64, u_int64_t,
                            void //need decrease bittness
                        >::type
                    >::type
                >::type
            >::type;
    };


    template <typename BitHolder>
    size_t ffz(BitHolder bits, size_t volume = sizeof(BitHolder)*8)
    {
        for(size_t sli(0); sli<volume; sli++)
        {
            BitHolder mask = ((BitHolder)1) << sli;
            if(bits & mask)
            {
                continue;
            }
            return sli;
        }

        return volume;
    }


    constexpr size_t bittness(size_t value)
    {
        return value ? bittness(value/2)+1 : 0;
    }
}}}}
#endif
