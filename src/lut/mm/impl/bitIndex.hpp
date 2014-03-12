#ifndef _LUT_MM_IMPL_BITINDEX_HPP_
#define _LUT_MM_IMPL_BITINDEX_HPP_

#include "lut/mm/impl/bitIndexLevel.hpp"
#include "lut/mm/config.hpp"
#include "lut/mm/impl/vm.hpp"

namespace lut { namespace mm { namespace impl
{

    ////////////////////////////////////////////////
    template <std::size_t maxVolume>
    class BitIndex
    {
    public:
        BitIndex();
        ~BitIndex();

    public:
        AddressInIndex alloc();
        void free(AddressInIndex address);
        bool isAllocated(AddressInIndex address);

    private:
        void updateProtection();

    private:
        static const std::size_t _bitIndexLevelBittness = 5;
        static const std::size_t _maxVolumeBittness = utils::bittness4Value(maxVolume-1);

        static const std::size_t _bitIndexLevelDepth =
                std::conditional<
                    (1 << (_maxVolumeBittness / _bitIndexLevelBittness * _bitIndexLevelBittness)) >= maxVolume,
                    std::integral_constant<std::size_t, _maxVolumeBittness / _bitIndexLevelBittness>,
                    std::integral_constant<std::size_t, _maxVolumeBittness / _bitIndexLevelBittness + 1> >::type::value;


        using SubLevel = BitIndexLevel<_bitIndexLevelBittness, _bitIndexLevelDepth, maxVolume>;

        std::size_t _protectedLayout;
        SubLevel _subLevel;
    };

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t maxVolume>
    BitIndex<maxVolume>::BitIndex()
        : _protectedLayout(vm::protect(this, Config::_pageSize, true) ? Config::_pageSize : 0)
    {

    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t maxVolume>
    BitIndex<maxVolume>::~BitIndex()
    {

    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t maxVolume>
    AddressInIndex BitIndex<maxVolume>::alloc()
    {
        bool layoutChanged = false;
        AddressInIndex res = _subLevel.alloc(layoutChanged);
        if(layoutChanged)
        {
            updateProtection();
        }
        return res;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t maxVolume>
    void BitIndex<maxVolume>::free(AddressInIndex address)
    {
        bool layoutChanged = false;
        _subLevel.free(address, layoutChanged);
        if(layoutChanged)
        {
            updateProtection();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t maxVolume>
    bool BitIndex<maxVolume>::isAllocated(AddressInIndex address)
    {
        return _subLevel.isAllocated(address);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t maxVolume>
    void BitIndex<maxVolume>::updateProtection()
    {
        std::size_t usedLayout = offsetof(BitIndex, _subLevel) + _subLevel.usedLayout();
        if(usedLayout % Config::_pageSize)
        {
            usedLayout = usedLayout - usedLayout % Config::_pageSize + Config::_pageSize;
        }

        char *base = reinterpret_cast<char *>(this);
        if(usedLayout > _protectedLayout)
        {
            vm::protect(base+_protectedLayout, usedLayout - _protectedLayout, true);
            _protectedLayout = usedLayout;
        }
        else if(usedLayout < _protectedLayout)
        {
            vm::protect(base+usedLayout, _protectedLayout - usedLayout, false);
            _protectedLayout = usedLayout;
        }
    }

}}}

#endif
