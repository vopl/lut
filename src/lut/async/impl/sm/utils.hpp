#ifndef _LUT_ASYNC_IMPL_SM_UTILS_HPP_
#define _LUT_ASYNC_IMPL_SM_UTILS_HPP_

namespace lut { namespace async { namespace impl { namespace utils
{
    template<std::size_t bittness, std::size_t bittness2=bittness>
    struct IntegralSelector
    {

        static const std::size_t max = bittness>bittness2 ? bittness : bittness2;

        using type =
            typename std::conditional<max <= 8, u_int8_t,
                typename std::conditional<max <= 16, u_int16_t,
                    typename std::conditional<max <= 32, u_int32_t,
                        typename std::conditional<max <= 64, u_int64_t,
                            void //need decrease bittness
                        >::type
                    >::type
                >::type
            >::type;
    };



    template <typename BitHolder>
    std::size_t ffz(BitHolder bits, std::size_t volume = sizeof(BitHolder)*8)
    {
        for(std::size_t sli(0); sli<volume; sli++)
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


}}}}
#endif
