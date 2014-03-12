#ifndef _LUT_ASYNC_IMPL_SM_INDEX_HPP_
#define _LUT_ASYNC_IMPL_SM_INDEX_HPP_

#include "lut/async/impl/sm/utils.hpp"

#include <cstddef>
#include <atomic>
#include <cassert>

namespace lut { namespace async { namespace impl { namespace sm
{
    ////////////////////////////////////////////////
    template <std::size_t levelBittness, std::size_t deepth, std::size_t bittnessConcurrency=8>
    class Index;

    ////////////////////////////////////////////////
    template <std::size_t levelBittness, std::size_t deepth, std::size_t bittnessConcurrency>
    class Index
    {
    public:
        std::size_t alloc();
        bool free(std::size_t address);
        bool isAllocated(std::size_t address);

    public:
        using SubIndex = Index<levelBittness, deepth-1, bittnessConcurrency>;
        static const std::size_t bittness = levelBittness + SubIndex::bittness;
        static const std::size_t volume = 1 << bittness;

    private:
        static const std::size_t sublevelsAmount = 1 << levelBittness;

        using Counter = typename utils::IntegralSelector<bittness+1, bittnessConcurrency>::type;
        using AtomicCounter = std::atomic<Counter>;
        AtomicCounter _counters[sublevelsAmount];

        SubIndex _sublevels[sublevelsAmount];
    };

    ////////////////////////////////////////////////////////////////
    template <std::size_t levelBittness, std::size_t bittnessConcurrency>
    class Index<levelBittness, 1, bittnessConcurrency>
    {
    public:
        std::size_t alloc();
        bool free(std::size_t address);
        bool isAllocated(std::size_t address);

    public:
        static const std::size_t bittness = levelBittness;
        static const std::size_t volume = 1 << bittness;

    private:
        using BitHolder = typename utils::IntegralSelector<volume>::type;
        std::atomic<BitHolder> _bitHolder;
    };











    ////////////////////////////////////////////////////////////////
    //N
    template <std::size_t levelBittness, std::size_t deepth, std::size_t bittnessConcurrency>
    std::size_t Index<levelBittness, deepth, bittnessConcurrency>::alloc()
    {
        for(std::size_t sli(0); sli<sublevelsAmount; sli++)
        {
            if(_counters[sli].fetch_add(1) < SubIndex::volume)
            {
                std::size_t res = _sublevels[sli].alloc();
                assert((std::size_t)-1 != res);
                return res | (sli << SubIndex::bittness);
            }
            _counters[sli].fetch_sub(1);
        }

        return -1;
    }

    ////////////////////////////////////////////////////////////////
    template <std::size_t levelBittness, std::size_t deepth, std::size_t bittnessConcurrency>
    bool Index<levelBittness, deepth, bittnessConcurrency>::free(std::size_t address)
    {
        assert(address < volume);

        const std::size_t sli = address >> SubIndex::bittness;
        const std::size_t subaddress = address & ((1 << SubIndex::bittness) -1);

        assert(_counters[sli].load());
        if(_sublevels[sli].free(subaddress))
        {
            _counters[sli].fetch_sub(1);
            return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////
    template <std::size_t levelBittness, std::size_t deepth, std::size_t bittnessConcurrency>
    bool Index<levelBittness, deepth, bittnessConcurrency>::isAllocated(std::size_t address)
    {
        assert(address < volume);

        const std::size_t sli = address >> SubIndex::bittness;
        const std::size_t subaddress = address & ((1 << SubIndex::bittness) -1);

        return _sublevels[sli].isAllocated(subaddress);
    }










    ////////////////////////////////////////////////////////////////
    //1
    template <std::size_t levelBittness, std::size_t bittnessConcurrency>
    std::size_t Index<levelBittness, 1, bittnessConcurrency>::alloc()
    {
        for(;;)
        {
            std::size_t sli = utils::ffz(_bitHolder.load(), volume);
            if(sli >= volume)
            {
                break;
            }

            BitHolder mask = ((BitHolder)1) << sli;
            if(_bitHolder.fetch_or(mask) & mask)
            {
                continue;
            }

            return sli;
        }

        return -1;
    }

    ////////////////////////////////////////////////////////////////
    template <std::size_t levelBittness, std::size_t bittnessConcurrency>
    bool Index<levelBittness, 1, bittnessConcurrency>::free(std::size_t address)
    {
        assert(address < volume);

        BitHolder mask = 1 << address;
        assert(_bitHolder & mask);
        return (_bitHolder.fetch_and(~mask) & mask) ? true : false;
    }

    ////////////////////////////////////////////////////////////////
    template <std::size_t levelBittness, std::size_t bittnessConcurrency>
    bool Index<levelBittness, 1, bittnessConcurrency>::isAllocated(std::size_t address)
    {
        assert(address < volume);

        BitHolder mask = 1 << address;
        return (_bitHolder.load() & mask) ? true : false;
    }
}}}}

#endif // INDEX_HPP
