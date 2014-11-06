#pragma once

#include "lut/mm/allocator/bitIndex.hpp"
#include "lut/mm/vm.hpp"
#include "lut/mm/utils/sized_cast.hpp"

namespace lut { namespace mm { namespace allocator
{
    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t volume>
    BitIndex<volume>::BitIndex()
    {
        vm::protect(this, ConfigMemory::_pageSize, true);
        _header._protectedSize = ConfigMemory::_pageSize;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t volume>
    BitIndex<volume>::~BitIndex()
    {
        vm::protect(this, sizeof(*this), false);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t volume>
    bitIndex::Address BitIndex<volume>::allocate()
    {
        bitIndex::Address addr = _topLevel.allocate();
        if(unlikely(bitIndex::_badAddress == addr))
        {
            assert(!"unable to allocate address, bitIndex full");
            std::abort();
        }

        if(unlikely(ConfigHeap::_blocksAmount == addr))
        {
            _topLevel.deallocate(addr);
            assert(!"unable to allocate address, blocks exhausted");
            std::abort();
        }

        if(addr > _header._maxAllocatedAddress)
        {
            updateProtection(addr);
        }

        return addr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t volume>
    bool BitIndex<volume>::isAllocated(bitIndex::Address address)
    {
        if(_header._maxAllocatedAddress < address)
        {
            return false;
        }

        return _topLevel.isAllocated(address);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t volume>
    void BitIndex<volume>::deallocate(bitIndex::Address address)
    {
        assert(_header._maxAllocatedAddress >= address);
        _topLevel.deallocate(address);

        if(_header._maxAllocatedAddress == address)
        {
            updateProtection(_topLevel.maxAllocatedAddress());
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t volume>
    void BitIndex<volume>::updateProtection(bitIndex::Address addr)
    {
        _header._maxAllocatedAddress = addr;

        std::size_t requiredArea = _topLevel.requiredAreaForAddress(addr) + offsetof(BitIndex<volume>, _topLevel);

        updateProtection(utils::sized_cast<char *>(this) + requiredArea);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t volume>
    void BitIndex<volume>::updateProtection(void *addr)
    {
        assert(addr > this && addr < this+1);
        std::size_t protectedSize = (static_cast<char *>(addr) - utils::sized_cast<char *>(this)) / ConfigMemory::_pageSize * ConfigMemory::_pageSize + ConfigMemory::_pageSize*2;

        if(protectedSize > _header._protectedSize)
        {
            if(!vm::protect(
                        utils::sized_cast<char *>(this) + _header._protectedSize,
                        protectedSize - _header._protectedSize,
                        true))
            {
                assert("unable to protect region");
                std::abort();
            }
            _header._protectedSize = protectedSize;
        }
        else if(protectedSize < _header._protectedSize - ConfigMemory::_pageSize)
        {
            if(!vm::protect(
                        utils::sized_cast<char *>(this) + protectedSize,
                        _header._protectedSize - protectedSize,
                        false))
            {
                assert("unable to protect region");
                std::abort();
            }
            _header._protectedSize = protectedSize;
        }
    }

}}}
