#ifndef _LUT_MM_IMPL_BITINDEX_HPP_
#define _LUT_MM_IMPL_BITINDEX_HPP_

#include <type_traits>
#include <cstddef>
#include <cassert>

namespace lut { namespace mm { namespace impl
{
    using AddressInIndex = size_t;
    static const AddressInIndex badAddressInIndex = (AddressInIndex)-1;

    ////////////////////////////////////////////////
    template <size_t levelBittness, size_t deepth>
    class BitIndex;

    ////////////////////////////////////////////////
    template <size_t levelBittness, size_t deepth>
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
        using SubIndex = BitIndex<levelBittness, deepth-1>;
        static const size_t bittness = levelBittness + SubIndex::bittness;
        static const size_t volume = 1 << bittness;
        static const size_t minMemoryRequired = offsetof(BitIndex, _sublevelsArea);

    private:
        static const size_t sublevelsAmount = 1 << levelBittness;

        using Counter = typename utils::IntegralSelector<bittness>::type;
        using CounterAndVolumeLimit = std::pair<Counter, Counter>;
        CounterAndVolumeLimit _counters[sublevelsAmount];

        size_t _volumeLimit;
        size_t _protectedAmount;

        //SubIndex _sublevels[sublevelsAmount];
        char _sublevelsArea[sizeof(SubIndex) * volume];
    };

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness>
    class BitIndex<levelBittness, 1>
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
        static const size_t minMemoryRequired = sizeof(BitHolder);

    private:
        using BitHolder = typename utils::IntegralSelector<volume>::type;
        BitHolder _bitHolder;
    };











    ////////////////////////////////////////////////////////////////
    //N
    template <size_t levelBittness, size_t deepth>
    AddressInIndex BitIndex<levelBittness, deepth>::alloc()
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
    template <size_t levelBittness, size_t deepth>
    bool BitIndex<levelBittness, deepth>::free(size_t address)
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
    template <size_t levelBittness, size_t deepth>
    bool BitIndex<levelBittness, deepth>::isAllocated(size_t address)
    {
        assert(address < volume);

        const size_t sli = address >> SubIndex::bittness;
        const AddressInIndex subaddress = address & ((1 << SubIndex::bittness) -1);

        return _sublevels[sli].isAllocated(subaddress);
    }










    ////////////////////////////////////////////////////////////////
    //1
    template <size_t levelBittness>
    size_t BitIndex<levelBittness, 1>::alloc()
    {
        for(;;)
        {
            size_t sli = utils::ffz(_bitHolder.load(), volume);
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
    template <size_t levelBittness>
    bool BitIndex<levelBittness, 1>::free(AddressInIndex address)
    {
        assert(address < volume);

        BitHolder mask = 1 << address;
        assert(_bitHolder & mask);
        return ((_bitHolder &= (~mask)) & mask) ? true : false;
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness>
    bool BitIndex<levelBittness, 1>::isAllocated(AddressInIndex address)
    {
        assert(address < volume);

        BitHolder mask = 1 << address;
        return (_bitHolder & mask) ? true : false;
    }
}}}

#endif
