#pragma once

#include "blocksHolder.hpp"
#include "../utils/sized_cast.hpp"

namespace dci { namespace mm { namespace allocator
{
    ////////////////////////////////////////////////////////////////////////////////
    inline BlocksHolder::~BlocksHolder()
    {
        for(Block *block = _empty.first(); block; block = _empty.first())
        {
            _empty.remove(block);
            Block::deallocateBlock(block);
        }
        assert(_empty.empty());
        assert(_middle.empty());
        assert(_full.empty());
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <std::size_t sizeClass> void *BlocksHolder::alloc()
    {
        Block *block = _middle.first();
        if(likely(block))
        {
            return block->alloc<sizeClass>();
        }

        block = _empty.first();
        if(likely(block))
        {
            return block->alloc<sizeClass>();
        }

        for(;;)
        {
            block = Block::allocateBlock(this, sizeClass);

            if(likely(block->empty()))
            {
                _empty.push(block);
                break;
            }
            else if(unlikely(block->full()))
            {
                _full.push(block);
            }
            else
            {
                _middle.push(block);
                break;
            }
        }

        assert(block);

        return block->alloc<sizeClass>();
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void *BlocksHolder::alloc(std::size_t sizeClass)
    {
        Block *block = _middle.first();
        if(likely(block))
        {
            return block->alloc();
        }

        block = _empty.first();
        if(likely(block))
        {
            return block->alloc();
        }

        for(;;)
        {
            block = Block::allocateBlock(this, sizeClass);

            if(likely(block->empty()))
            {
                _empty.push(block);
                break;
            }
            else if(unlikely(block->full()))
            {
                _full.push(block);
            }
            else
            {
                _middle.push(block);
                break;
            }
        }

        assert(block);

        return block->alloc();
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void BlocksHolder::blockEmptyToMiddle(Block *block)
    {
        assert(_empty.contain(block));

        _empty.remove(block);
        _middle.push(block);
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void BlocksHolder::blockMiddleToEmpty(Block *block)
    {
        assert(_middle.contain(block));

        _middle.remove(block);

        if(_empty.empty())
        {
            _empty.push(block);
        }
        else
        {
            Block::deallocateBlock(block);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void BlocksHolder::blockMiddleToFull(Block *block)
    {
        assert(_middle.contain(block));

        _middle.remove(block);
        _full.push(block);
    }

    ////////////////////////////////////////////////////////////////////////////////
    inline void BlocksHolder::blockFullToMiddle(Block *block)
    {
        assert(_full.contain(block));

        _full.remove(block);
        _middle.push(block);
    }

}}}
