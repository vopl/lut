#ifndef _LUT_ASYNC_IMPL_SM_UTILS_HPP_
#define _LUT_ASYNC_IMPL_SM_UTILS_HPP_

namespace lut { namespace mm { namespace impl { namespace utils
{
    template<size_t bittness4Value>
    struct Integral4Bittness
    {

        using type =
            typename std::conditional<bittness4Value <= 8, u_int8_t,
                typename std::conditional<bittness4Value <= 16, u_int16_t,
                    typename std::conditional<bittness4Value <= 32, u_int32_t,
                        typename std::conditional<bittness4Value <= 64, u_int64_t,
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


    constexpr size_t bittness4Value(size_t value)
    {
        return value ? bittness4Value(value>>1)+1 : 0;
    }
}}}}
#endif
