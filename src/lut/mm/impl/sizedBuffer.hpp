#ifndef _LUT_MM_IMPL_SIZEDBUFFER_HPP_
#define _LUT_MM_IMPL_SIZEDBUFFER_HPP_

#include "lut/mm/impl/buffer.hpp"
#include "lut/mm/impl/vm.hpp"

#include <cstdint>

namespace lut { namespace mm { namespace impl
{
    template <std::size_t size>
    class SizedBuffer
        : public Buffer
    {
    public:
        SizedBuffer();
        ~SizedBuffer();

    public:
        template <typename BufferContainer>
        void *alloc(BufferContainer *bufferContainer);

        template <typename BufferContainer>
        void free(void *ptr, BufferContainer *bufferContainer);

    private:
        using Offset = std::uint_fast32_t;

        struct Block
        {
            using Data = typename std::aligned_storage<size < sizeof(Offset) ? sizeof(Offset) : size, 1>::type;
            Data _data;

            Offset &next();
        };

        static const Offset _badOffset = (Offset)-1;

        struct Header
        {
            Offset _next;
            Offset _initialized;
            Offset _allocated;

            Header();
        };
        Header &header();
        Block *blocks();

        Block* offset2Block(Offset i);
        Offset block2Offset(Block* p);

        static const std::size_t _blocksOffset = _areaOffset + sizeof(typename std::aligned_storage<sizeof(Header), alignof(Header)>::type);
        static const std::size_t _blocksAmount = (sizeof(Buffer) - _blocksOffset) / sizeof(Block);

    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::SizedBuffer()
    {
        assert(!(reinterpret_cast<std::uintptr_t>(this) % Config::_pageSize));
        std::size_t protectedBound = sizeof(typename std::aligned_storage<_blocksOffset, Config::_pageSize>::type);
        vm::protect(this, protectedBound, true);

        new (&header()) Header;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::~SizedBuffer()
    {
        header().~Header();
        vm::protect(this, sizeof(Buffer), false);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    template <typename BufferContainer>
    void *SizedBuffer<size>::alloc(BufferContainer *bufferContainer)
    {
        assert(header()._next <= _blocksAmount*sizeof(Block));
        assert(header()._initialized <= _blocksAmount*sizeof(Block));
        assert(header()._allocated <= _blocksAmount*sizeof(Block));

        if(header()._allocated >= _blocksAmount)
        {
            assert(header()._next == _blocksAmount);
            return nullptr;
        }
        assert(header()._next < _blocksAmount);

        Block *block = offset2Block(header()._next);

        if(header()._allocated == header()._initialized)
        {
            std::size_t protect = _blocksOffset + header()._initialized * sizeof(Block) + sizeof(Block);
            if((protect % Config::_pageSize) + sizeof(Block) >= Config::_pageSize)
            {
                vm::protect(
                            reinterpret_cast<char *>(this) + protect - (protect % Config::_pageSize),
                            Config::_pageSize,
                            true);
            }
            header()._next++;
            header()._initialized++;
        }
        else
        {
            header()._next = block->next();
        }

        header()._allocated++;

        switch(header()._allocated)
        {
        case 1:
            bufferContainer->template bufferEmpty2Middle<size>(this);
            break;
        case _blocksAmount:
            bufferContainer->template bufferMiddle2Full<size>(this);
            break;
        }

        return block;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    template <typename BufferContainer>
    void SizedBuffer<size>::free(void *ptr, BufferContainer *bufferContainer)
    {
        assert(ptr >= blocks() && ptr < (blocks()+_blocksAmount));
        assert(!(((char *)ptr - (char *)blocks()) % sizeof(Block)));

        Block *block = reinterpret_cast<Block *>(ptr);
        Offset blockOffset = block2Offset(block);

        block->next() = header()._next;
        header()._next = blockOffset;
        header()._allocated--;

        switch(header()._allocated)
        {
        case 0:
            bufferContainer->template bufferMiddle2Empty<size>(this);
            break;
        case _blocksAmount-1:
            bufferContainer->template bufferFull2Middle<size>(this);
            break;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Offset &SizedBuffer<size>::Block::next()
    {
        return *reinterpret_cast<Offset*>(&_data);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::Header::Header()
        : _next()
        , _initialized()
        , _allocated()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Header &SizedBuffer<size>::header()
    {
        return *reinterpret_cast<Header*>(&_area);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Block *SizedBuffer<size>::blocks()
    {
        return reinterpret_cast<Block *>(reinterpret_cast<char *>(this) + _blocksOffset);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Block *SizedBuffer<size>::offset2Block(Offset i)
    {
        return blocks() + i;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Offset SizedBuffer<size>::block2Offset(Block* p)
    {
        return p - blocks();
    }

}}}

#endif
