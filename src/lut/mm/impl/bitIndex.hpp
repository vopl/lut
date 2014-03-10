#ifndef _LUT_MM_IMPL_BITINDEX_HPP_
#define _LUT_MM_IMPL_BITINDEX_HPP_

#include "lut/mm/impl/utils.hpp"

#include <type_traits>
#include <cstddef>
#include <cassert>

namespace lut { namespace mm { namespace impl
{
    using AddressInIndex = size_t;
    static const AddressInIndex badAddressInIndex = (AddressInIndex)-1;

    ////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t amount>
    class BitIndex;

    ////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t amount>
    class BitIndex
    {
    public:
        BitIndex(size_t volumeLimit, size_t protectedAmount);
        ~BitIndex();

    public:
        AddressInIndex alloc();
        bool free(AddressInIndex address);
        bool isAllocated(AddressInIndex address);

    public:
        static const size_t bittness = levelBittness * depth;
        static const size_t volume = 1 << bittness;
        static const size_t sublevelsAmount = 1 << levelBittness;

        using SubIndex = BitIndex<levelBittness, depth-1, amount/sublevelsAmount>;
    private:

        using Counter = typename utils::IntegralSelector<bittness>::type;
        Counter _counters[sublevelsAmount];

        SubIndex _sublevels[sublevelsAmount];
    };

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t amount>
    class BitIndex<levelBittness, 1, amount>
    {
    public:
        BitIndex(size_t volumeLimit, size_t protectedAmount);
        ~BitIndex();

    public:
        AddressInIndex alloc();
        bool free(AddressInIndex address);
        bool isAllocated(AddressInIndex address);

    public:
        static const size_t bittness = levelBittness;
        static const size_t volume = 1 << bittness;

    private:
        using BitHolder = typename utils::IntegralSelector<volume>::type;
        BitHolder _bitHolder;
    };











    ////////////////////////////////////////////////////////////////
    //N
    template <size_t levelBittness, size_t depth, size_t amount>
    AddressInIndex BitIndex<levelBittness, depth, amount>::alloc()
    {
        for(size_t sli(0); sli<sublevelsAmount; sli++)
        {
            if((_counters[sli] += 1) < SubIndex::volume)
            {
                AddressInIndex res = _sublevels[sli].alloc();
                assert((AddressInIndex)-1 != res);
                return res | (sli << SubIndex::bittness);
            }
            _counters[sli] -= 1;
        }

        return -1;
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t amount>
    bool BitIndex<levelBittness, depth, amount>::free(size_t address)
    {
        assert(address < volume);

        const size_t sli = address >> SubIndex::bittness;
        const AddressInIndex subaddress = address & ((1 << SubIndex::bittness) -1);

        assert(_counters[sli].load());
        if(_sublevels[sli].free(subaddress))
        {
            _counters[sli].fetch_sub(1);
            return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t amount>
    bool BitIndex<levelBittness, depth, amount>::isAllocated(size_t address)
    {
        assert(address < volume);

        const size_t sli = address >> SubIndex::bittness;
        const AddressInIndex subaddress = address & ((1 << SubIndex::bittness) -1);

        return _sublevels[sli].isAllocated(subaddress);
    }










    ////////////////////////////////////////////////////////////////
    //1
    template <size_t levelBittness, size_t amount>
    size_t BitIndex<levelBittness, 1, amount>::alloc()
    {
        for(;;)
        {
            size_t sli = utils::ffz(_bitHolder, volume);
            if(sli >= volume)
            {
                break;
            }

            BitHolder mask = ((BitHolder)1) << sli;
            if((_bitHolder |= mask) & mask)
            {
                continue;
            }

            return sli;
        }

        return -1;
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t amount>
    bool BitIndex<levelBittness, 1, amount>::free(AddressInIndex address)
    {
        assert(address < volume);

        BitHolder mask = 1 << address;
        assert(_bitHolder & mask);
        return ((_bitHolder &= (~mask)) & mask) ? true : false;
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t amount>
    bool BitIndex<levelBittness, 1, amount>::isAllocated(AddressInIndex address)
    {
        assert(address < volume);

        BitHolder mask = 1 << address;
        return (_bitHolder & mask) ? true : false;
    }
}}}

#endif
