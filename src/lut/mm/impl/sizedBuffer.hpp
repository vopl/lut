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
        struct Block
        {
            using Data = typename std::aligned_storage<(size < sizeof(Offset) ? sizeof(Offset) : size), 1>::type;
            Data _data;

            Offset &next();
        };

        char *blocksArea();

        Block* offset2Block(Offset i);
        Offset block2Offset(Block* p);

        static const std::size_t _blocksAlign =
            sizeof(Block) <=1 ? 1 :
            sizeof(Block) <=2 ? 2 :
            sizeof(Block) <=4 ? 4 :
            sizeof(Block) <=8 ? 8 : 16;

        static const std::size_t _blocksOffset = (_headerSize % _blocksAlign) ? (_headerSize - _headerSize % _blocksAlign + _blocksAlign) : _headerSize;
        static const std::size_t _blocksAmount = (sizeof(Buffer) - _blocksOffset) / sizeof(Block);
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::SizedBuffer()
    {
        assert(!(reinterpret_cast<std::uintptr_t>(this) % Config::_pageSize));
        std::size_t protectedBound = sizeof(typename std::aligned_storage<_blocksOffset, Config::_pageSize>::type);
        vm::protect(this, protectedBound, true);

        _areaAddress = reinterpret_cast<char *>(&_area);
        _allocated = _next = _initialized = 0;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::~SizedBuffer()
    {
        vm::protect(this, sizeof(Buffer), false);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    template <typename BufferContainer>
    void *SizedBuffer<size>::alloc(BufferContainer *bufferContainer)
    {

        assert(_next <= _blocksAmount*sizeof(Block));
        assert(_initialized <= _blocksAmount*sizeof(Block));
        assert(_allocated <= _blocksAmount*sizeof(Block));

        if(_allocated >= (_blocksAmount-1)*sizeof(Block))
        {
            if(_allocated >= _blocksAmount*sizeof(Block))
            {
                assert(_next == _blocksAmount*sizeof(Block));
                return nullptr;
            }

            Block *block = offset2Block(_next);
            assert((reinterpret_cast<std::uintptr_t>(block) & 0xfff)!=8);

            assert(_allocated == _initialized);
            _allocated = _blocksAmount*sizeof(Block);
            _next = block->next();

            bufferContainer->template bufferMiddle2Full<size>(this);
            return block;
        }
        assert(_next < _blocksAmount*sizeof(Block));

        Block *block = offset2Block(_next);

        if(_allocated == _initialized)
        {
            std::size_t protect = _blocksOffset + _initialized;
            if((protect % Config::_pageSize) + sizeof(Block) >= Config::_pageSize)
            {
                vm::protect(
                            reinterpret_cast<char *>(this) + protect - (protect % Config::_pageSize) + Config::_pageSize,
                            Config::_pageSize,
                            true);
            }

            _allocated = _next = _initialized =
                _initialized + sizeof(Block);
        }
        else
        {
            _allocated += sizeof(Block);
            _next = block->next();
        }

        if(1*sizeof(Block) == _allocated)
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
        assert(ptr >= blocksArea() && ptr < (blocksArea()+_blocksAmount*sizeof(Block)));
        assert(!(((char *)ptr - blocksArea()) % sizeof(Block)));

        Block *block = reinterpret_cast<Block *>(ptr);

        block->next() = _next;
        _allocated -= sizeof(Block);
        _next = block2Offset(block);

        if(_allocated!=sizeof(Block))
        {
            if(_allocated!=(_blocksAmount-1)*sizeof(Block))
            {
                //empty
            }
            else
            {
                bufferContainer->template bufferFull2Middle<size>(this);
            }
        }
        else
        {
            bufferContainer->template bufferMiddle2Empty<size>(this);
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
    char *SizedBuffer<size>::blocksArea()
    {
        return _areaAddress;
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
