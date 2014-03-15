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
        char *blocksArea();

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
        Header &header = SizedBuffer::header();

        assert(header._next <= _blocksAmount*sizeof(Block));
        assert(header._initialized <= _blocksAmount*sizeof(Block));
        assert(header._allocated <= _blocksAmount*sizeof(Block));

        if(header._allocated >= (_blocksAmount-1)*sizeof(Block))
        {
            if(header._allocated >= _blocksAmount*sizeof(Block))
            {
                assert(header._next == _blocksAmount*sizeof(Block));
                return nullptr;
            }

            Block *block = offset2Block(header._next);
            assert((reinterpret_cast<std::uintptr_t>(block) & 0xfff)!=8);

            assert(header._allocated == header._initialized);
            header._next = block->next();
            header._allocated = _blocksAmount*sizeof(Block);

            bufferContainer->template bufferMiddle2Full<size>(this);
            return block;
        }
        assert(header._next < _blocksAmount*sizeof(Block));

        Block *block = offset2Block(header._next);

        if(header._allocated == header._initialized)
        {
            std::size_t protect = _blocksOffset + header._initialized;
            if((protect % Config::_pageSize) + sizeof(Block) >= Config::_pageSize)
            {
                vm::protect(
                            reinterpret_cast<char *>(this) + protect - (protect % Config::_pageSize) + Config::_pageSize,
                            Config::_pageSize,
                            true);
            }

            header._next += sizeof(Block);
            header._initialized += sizeof(Block);
        }
        else
        {
            header._next = block->next();
        }

        header._allocated += sizeof(Block);
        if(1*sizeof(Block) == header._allocated)
        {
            bufferContainer->template bufferEmpty2Middle<size>(this);
        }

        return block;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    template <typename BufferContainer>
    void SizedBuffer<size>::free(void *ptr, BufferContainer *bufferContainer)
    {
        Header &header = SizedBuffer::header();

        assert(ptr >= blocksArea() && ptr < (blocksArea()+_blocksAmount*sizeof(Block)));
        assert(!(((char *)ptr - blocksArea()) % sizeof(Block)));

        Block *block = reinterpret_cast<Block *>(ptr);

        block->next() = header._next;
        header._next = block2Offset(block);
        header._allocated -= sizeof(Block);

        if(0*sizeof(Block) == header._allocated)
        {
            bufferContainer->template bufferMiddle2Empty<size>(this);
        }
        else if((_blocksAmount-1)*sizeof(Block) == header._allocated)
        {
            bufferContainer->template bufferFull2Middle<size>(this);
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
    char *SizedBuffer<size>::blocksArea()
    {
        return reinterpret_cast<char *>(this) + _blocksOffset;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Block *SizedBuffer<size>::offset2Block(Offset i)
    {
        return reinterpret_cast<Block*>(blocksArea() + i);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Offset SizedBuffer<size>::block2Offset(Block* p)
    {
        return reinterpret_cast<char *>(p) - blocksArea();
    }

}}}

#endif
