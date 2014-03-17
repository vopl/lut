#ifndef _LUT_MM_IMPL_SIZEDBUFFER_HPP_
#define _LUT_MM_IMPL_SIZEDBUFFER_HPP_

#include "lut/mm/impl/buffer.hpp"
#include "lut/mm/impl/vm.hpp"

#include <cstdint>
#include <cstddef>

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
        union alignas(size <= alignof(void *) ? alignof(void *) : alignof(/*std::max_align_t*/long double)) Block
        {
            Block *_next;
            char _data[size];
        };

        Block *blocksArea();
        Block *next();
        void next(Block *);

        static const std::size_t _blocksAlign = alignof(Block);

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

        _areaAddress = reinterpret_cast<char *>(this) + _blocksOffset;
        next(blocksArea());
        _allocated = _initialized = 0;
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

        assert(next() >= blocksArea() && next() <= blocksArea() + _blocksAmount);
        assert(_initialized <= _blocksAmount);
        assert(_allocated <= _blocksAmount);

        if(_allocated >= (_blocksAmount-1))
        {
            if(_allocated >= _blocksAmount)
            {
                assert(next() == blocksArea() + _blocksAmount);
                return nullptr;
            }

            Block *block = next();
            assert((reinterpret_cast<std::uintptr_t>(block) & 0xfff)!=8);

            assert(_allocated == _initialized);
            _allocated = _blocksAmount;
            next(block->_next);

            bufferContainer->template bufferMiddle2Full<size>(this);
            return block;
        }

        Block *block = next();

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

            next(blocksArea() + _allocated);
            _initialized++;
        }
        else
        {
            next(block->_next);
        }

        if(!_allocated++)
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
        assert(!(((char *)ptr - (char *)blocksArea()) % sizeof(Block)));

        Block *block = reinterpret_cast<Block *>(ptr);

        block->_next = next();
        _allocated -= 1;
        next(block);

        if(_allocated!=1)
        {
            if(_allocated!=(_blocksAmount-1))
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
    typename SizedBuffer<size>::Block *SizedBuffer<size>::blocksArea()
    {
        return reinterpret_cast<Block *>(_areaAddress);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Block *SizedBuffer<size>::next()
    {
        return reinterpret_cast<Block *>(_next);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    void SizedBuffer<size>::next(Block *block)
    {
        _next = reinterpret_cast<Offset>(block);
    }

}}}

#endif
