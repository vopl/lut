#pragma once

#include "allocator.hpp"
#include "utils/sized_cast.hpp"
#include "vm.hpp"
#include "likely.hpp"

#include <malloc.h>

namespace dci { namespace mm
{
    template <std::size_t sizeClass>
    void *Allocator::alloc()
    {
        const std::size_t sizeClassIndex = ConfigHeap::evalSizeClassIndex(sizeClass);
        return _blocksHolders[sizeClassIndex].alloc((sizeClassIndex+1)*ConfigHeap::_sizeGranula);
    }

    template <std::size_t sizeClass> void Allocator::free(void *ptr)
    {
#ifndef NDEBUG
        allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(ptr) - utils::sized_cast<char *>(_blocks)) / ConfigHeap::_blockSize;
        assert(_blocksBitIndex->isAllocated(blockBitAddr));
#endif

        void *blockPtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(ptr) / ConfigHeap::_blockSize * ConfigHeap::_blockSize);
        allocator::Block *block = utils::sized_cast<allocator::Block *>(blockPtr);

        block->free<sizeClass>(ptr);
    }

    inline void *Allocator::alloc(std::size_t size)
    {
        std::size_t sizeClassIndex;
        if(unlikely(!size))
        {
            sizeClassIndex = 0;
        }
        else
        {
            sizeClassIndex = (size-1)/ConfigHeap::_sizeGranula;

            if(unlikely(sizeClassIndex > ConfigHeap::_maxSizeClassIndex))
            {
                return system::malloc(size);
            }
        }

        return _blocksHolders[sizeClassIndex].alloc((sizeClassIndex+1)*ConfigHeap::_sizeGranula);
    }

    inline void Allocator::free(void *ptr)
    {
        if(unlikely(utils::sized_cast<std::size_t>(ptr) - utils::sized_cast<std::size_t>(_blocks) >= utils::sized_cast<std::size_t>(ConfigHeap::_blocksAmount * ConfigHeap::_blockSize)))
        {
            return system::free(ptr);
        }

#ifndef NDEBUG
        allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(ptr) - utils::sized_cast<char *>(_blocks)) / ConfigHeap::_blockSize;
        assert(_blocksBitIndex->isAllocated(blockBitAddr));
#endif

        void *blockPtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(ptr) / ConfigHeap::_blockSize * ConfigHeap::_blockSize);
        allocator::Block *block = utils::sized_cast<allocator::Block *>(blockPtr);

        block->free(ptr);
    }

    inline std::size_t Allocator::size(void *ptr)
    {
        if(unlikely(utils::sized_cast<std::size_t>(ptr) - utils::sized_cast<std::size_t>(_blocks) >= utils::sized_cast<std::size_t>(ConfigHeap::_blocksAmount * ConfigHeap::_blockSize)))
        {
            return ::malloc_usable_size(ptr);
        }

#ifndef NDEBUG
        allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(ptr) - utils::sized_cast<char *>(_blocks)) / ConfigHeap::_blockSize;
        assert(_blocksBitIndex->isAllocated(blockBitAddr));
#endif

        void *blockPtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(ptr) / ConfigHeap::_blockSize * ConfigHeap::_blockSize);
        allocator::Block *block = utils::sized_cast<allocator::Block *>(blockPtr);

        return block->getSizeClass();
    }

    inline void *Allocator::allocateBlock()
    {
        if(unlikely(!_vm))
        {
            init();
        }
        assert(_blocksBitIndex && _blocks);

        allocator::bitIndex::Address bitAddr = _blocksBitIndex->allocate();
        return static_cast<char *>(_blocks) + bitAddr * ConfigHeap::_blockSize;
    }

    inline void Allocator::deallocateBlock(void *ptr)
    {
        assert(_blocksBitIndex && _blocks);

        allocator::bitIndex::Address bitAddr = (static_cast<char *>(ptr) - static_cast<char *>(_blocks)) / ConfigHeap::_blockSize;
        assert(_blocksBitIndex->isAllocated(bitAddr));
        assert(ptr == static_cast<char *>(_blocks) + bitAddr * ConfigHeap::_blockSize);
        _blocksBitIndex->deallocate(bitAddr);
    }
}}
