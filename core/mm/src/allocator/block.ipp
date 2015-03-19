#pragma once

#include "../utils/sized_cast.hpp"
#include "block.hpp"
#include "../allocator.hpp"
#include "../vm.hpp"

#include <cstdlib>
#include <new>

namespace dci { namespace mm { namespace allocator
{

    ////////////////////////////////////////////////////////////////////////////////
    inline Block *Block::allocateBlock(BlocksHolder *thisBlockHolder, std::size_t sizeClass)
    {
        void *ptr = g_allocator.allocateBlock();
        if(!ptr)
        {
            return nullptr;
        }

        if(!vm::protect(ptr, ConfigHeap::_blockSize, true))
        {
            std::abort();
        }

        Block *block = new(ptr) Block(thisBlockHolder, sizeClass);
        return block;
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void Block::deallocateBlock(Block *block)
    {
        block->~Block();
        g_allocator.deallocateBlock(block);

        if(!vm::protect(block, sizeof(Block), false))
        {
            std::abort();
        }

    }

    ////////////////////////////////////////////////////////////////////////////////
    inline Block::Block(BlocksHolder *thisBlockHolder, std::size_t sizeClass)
        : _nextFreeElement{utils::sized_cast<ElementHeader *>(_elementsArea)}
        , _next{}
        , _prev{}
        , _thisBlockHolder{thisBlockHolder}
        , _allocated{0}
        , _sizeClass{sizeClass}
        , _elementsAmount{(ConfigHeap::_blockSize - offsetof(Block, _elementsArea)) / sizeClass}
    {
        const std::size_t elementsAmount = (ConfigHeap::_blockSize - offsetof(Block, _elementsArea)) / _sizeClass;

        ElementHeader *element = this->_nextFreeElement;
        for(std::size_t i{0}; i<elementsAmount; ++i)
        {
            element->_next = utils::sized_cast<ElementHeader *>(utils::sized_cast<char *>(element)+_sizeClass);
            element = element->_next;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline Block::~Block()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void *Block::allocWithoutCounters()
    {
        ElementHeader *element = _nextFreeElement;
        _nextFreeElement = element->_next;
        return element;
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void Block::freeWithoutCounters(void *ptr)
    {
        ElementHeader *element = utils::sized_cast<ElementHeader *>(ptr);

        element->_next = _nextFreeElement;
        _nextFreeElement = element;
        assert(reinterpret_cast<std::size_t>(_nextFreeElement) < reinterpret_cast<std::size_t>(this) + ConfigHeap::_blockSize);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t sizeClass> void *Block::alloc()
    {
        if(unlikely(0 == _allocated))
        {
            _allocated++;
            this->_thisBlockHolder->blockEmptyToMiddle(this);
        }
        else
        {
            std::size_t elementsAmount = (ConfigHeap::_blockSize - offsetof(Block, _elementsArea)) / sizeClass;
            assert(_elementsAmount == elementsAmount);

            if(unlikely(elementsAmount-1 == _allocated))
            {
                _allocated++;
                this->_thisBlockHolder->blockMiddleToFull(this);
            }
        }

        return allocWithoutCounters();
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void *Block::alloc()
    {
        {
            _allocated++;

            if(unlikely(1 == _allocated))
            {
                this->_thisBlockHolder->blockEmptyToMiddle(this);
            }
            else
            {
                if(unlikely(_elementsAmount == _allocated))
                {
                    this->_thisBlockHolder->blockMiddleToFull(this);
                }
            }
        }

        return allocWithoutCounters();
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t sizeClass> void Block::free(void *ptr)
    {
        freeWithoutCounters(ptr);

        _allocated--;
        if(unlikely(0 == _allocated))
        {
            this->_thisBlockHolder->blockMiddleToEmpty(this);
        }
        else
        {
            std::size_t elementsAmount = (ConfigHeap::_blockSize - offsetof(Block, _elementsArea)) / sizeClass;
            assert(_elementsAmount == elementsAmount);

            if(unlikely(elementsAmount-1 == _allocated))
            {
                this->_thisBlockHolder->blockFullToMiddle(this);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void Block::free(void *ptr)
    {
        freeWithoutCounters(ptr);

        if(unlikely(1 == _allocated))
        {
            _allocated--;
            this->_thisBlockHolder->blockMiddleToEmpty(this);
        }
        else
        {
            if(unlikely(_elementsAmount == _allocated))
            {
                _allocated--;
                this->_thisBlockHolder->blockFullToMiddle(this);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline bool Block::empty() const
    {
        return 0 == _allocated;
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline bool Block::full() const
    {
        return _elementsAmount == _allocated;
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline std::size_t Block::getSizeClass() const
    {
        return _sizeClass;
    }

}}}
