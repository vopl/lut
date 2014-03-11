#ifndef _LUT_MM_IMPL_BITINDEXLEVEL_HPP_
#define _LUT_MM_IMPL_BITINDEXLEVEL_HPP_

#include "lut/mm/impl/utils.hpp"

#include <type_traits>
#include <cstddef>
#include <cassert>

namespace lut { namespace mm { namespace impl
{
    using AddressInIndex = size_t;
    static const AddressInIndex badAddressInIndex = (AddressInIndex)-1;

    ////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t maxVolume>
    class BitIndexLevel;

    ////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t maxVolume>
    class BitIndexLevel
    {
    public:
        AddressInIndex alloc(bool &layoutChanged);
        void free(AddressInIndex address, bool &layoutChanged);
        bool isAllocated(AddressInIndex address) const;
        size_t usedLayout() const;

    public:
        static const size_t _bittness = levelBittness * depth;
        static const size_t _volume = ((size_t)1 << _bittness) > maxVolume ? maxVolume : ((size_t)1 << _bittness);
        static const size_t _subLevelsAmount = 1 << levelBittness;

        using SubLevel = BitIndexLevel<
            levelBittness,
            depth-1,
            _volume/_subLevelsAmount + (_volume % _subLevelsAmount ? 1 : 0)>;

        using SubLevelLast = BitIndexLevel<
            levelBittness,
            depth-1,
            _volume - SubLevel::_volume*(_subLevelsAmount-1)>;

    private:

        using Counter = typename utils::Integral4Bittness<_bittness>::type;
        Counter _counters[_subLevelsAmount];

        using WorkerIndex = typename utils::Integral4Bittness<levelBittness>::type;
        WorkerIndex _workIndex;
        WorkerIndex _lastUsedIndex;

        SubLevel        _subLevels[_subLevelsAmount-1];
        SubLevelLast    _subLevelLast;
    };

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t maxVolume>
    class BitIndexLevel<levelBittness, 1, maxVolume>
    {
    public:
        AddressInIndex alloc(bool &layoutChanged);
        void free(AddressInIndex address, bool &layoutChanged);
        bool isAllocated(AddressInIndex address) const;
        size_t usedLayout() const;

    public:
        static const size_t _bittness = levelBittness;
        static const size_t _volume = ((size_t)1 << _bittness) > maxVolume ? maxVolume : ((size_t)1 << _bittness);

    private:
        using BitHolder = typename utils::Integral4Bittness<_volume>::type;
        BitHolder _bitHolder;
    };











    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t maxVolume>
    AddressInIndex BitIndexLevel<levelBittness, depth, maxVolume>::alloc(bool &layoutChanged)
    {
        if(_workIndex >= _subLevelsAmount)
        {
            return badAddressInIndex;
        }

        if(_workIndex == _subLevelsAmount-1)
        {
            AddressInIndex res = _subLevels[_workIndex].alloc(layoutChanged);
            assert(badAddressInIndex != res);
            res |= (_workIndex << SubLevel::_bittness);

            ++_counters[_workIndex];
            assert(_counters[_workIndex] <= SubLevelLast::_volume);
            if(_counters[_workIndex] == SubLevelLast::_volume)
            {
                ++_workIndex;
            }

            return res;
        }

        AddressInIndex res = _subLevels[_workIndex].alloc(layoutChanged);
        assert(badAddressInIndex != res);
        res |= (_workIndex << SubLevel::_bittness);

        ++_counters[_workIndex];
        while(_workIndex <_subLevelsAmount-1)
        {
            assert(_counters[_workIndex] <= SubLevel::_volume);
            if(_counters[_workIndex] == SubLevel::_volume)
            {
                ++_workIndex;
                continue;
            }

            if(_lastUsedIndex < _workIndex)
            {
                layoutChanged = true;
                _lastUsedIndex = _workIndex;
            }

            return res;
        }

        assert(_subLevelsAmount-1 == _workIndex);
        if(_counters[_workIndex] == SubLevelLast::_volume)
        {
            ++_workIndex;
        }

        return res;
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t maxVolume>
    void BitIndexLevel<levelBittness, depth, maxVolume>::free(size_t address, bool &layoutChanged)
    {
        assert(address < _volume);

        const size_t sli = address >> SubLevel::_bittness;
        const AddressInIndex subaddress = address & ((1 << SubLevel::_bittness) -1);

        assert(_counters[sli]);
        --_counters[sli];

        if(_workIndex > sli)
        {
            _workIndex = sli;
        }

        {
            size_t sli2 = sli;
            while(sli2 && !_counters[sli2])
            {
                --sli2;
            }

            if(sli2 != _lastUsedIndex)
            {
                layoutChanged = true;
                _lastUsedIndex = sli2;
            }
        }


        if(sli == _subLevelsAmount-1)
        {
            _subLevelLast.free(subaddress, layoutChanged);
            return;
        }

        _subLevels[sli].free(subaddress, layoutChanged);
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t maxVolume>
    bool BitIndexLevel<levelBittness, depth, maxVolume>::isAllocated(size_t address) const
    {
        assert(address < _volume);

        const size_t sli = address >> SubLevel::_bittness;
        const AddressInIndex subaddress = address & ((1 << SubLevel::_bittness) -1);

        if(sli == _subLevelsAmount-1)
        {
            return _subLevelLast.isAllocated(subaddress);
        }

        return _subLevels[sli].isAllocated(subaddress);
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t depth, size_t maxVolume>
    size_t BitIndexLevel<levelBittness, depth, maxVolume>::usedLayout() const
    {
        if(_lastUsedIndex == _subLevelsAmount-1)
        {
            return offsetof(BitIndexLevel, _subLevelLast) + _subLevelLast.usedLayout();
        }

        return offsetof(BitIndexLevel, _subLevels[_lastUsedIndex]) + _subLevels[_lastUsedIndex].usedLayout();
    }









    ////////////////////////////////////////////////////////////////
    //1
    template <size_t levelBittness, size_t maxVolume>
    AddressInIndex BitIndexLevel<levelBittness, 1, maxVolume>::alloc(bool &layoutChanged)
    {
        (void)layoutChanged;
        size_t sli = utils::ffz(_bitHolder, _volume);
        assert(sli < _volume);
        if(sli >= _volume)
        {
            return badAddressInIndex;
        }

        _bitHolder |= ((BitHolder)1) << sli;
        return static_cast<AddressInIndex>(sli);
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t maxVolume>
    void BitIndexLevel<levelBittness, 1, maxVolume>::free(AddressInIndex address, bool &layoutChanged)
    {
        (void)layoutChanged;
        assert(address < _volume);

        BitHolder mask = 1 << address;
        assert(_bitHolder & mask);
        _bitHolder &= (~mask);
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t maxVolume>
    bool BitIndexLevel<levelBittness, 1, maxVolume>::isAllocated(AddressInIndex address) const
    {
        assert(address < _volume);

        BitHolder mask = 1 << address;
        return (_bitHolder & mask) ? true : false;
    }

    ////////////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t maxVolume>
    size_t BitIndexLevel<levelBittness, 1, maxVolume>::usedLayout() const
    {
        return sizeof(BitIndexLevel);
    }

}}}

#endif
