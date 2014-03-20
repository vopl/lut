#ifndef _LUT_MM_IMPL_SIZEDBUFFER_HPP_
#define _LUT_MM_IMPL_SIZEDBUFFER_HPP_

#include "lut/mm/impl/buffer.hpp"
#include "lut/mm/impl/vm.hpp"
#include "lut/mm/impl/branchHint.hpp"

#include <cstdint>
#include <cstddef>
#include <atomic>
#include <xmmintrin.h>

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

        template <typename BufferContainer>
        void freeFromOtherThread(void *ptr, BufferContainer *bufferContainer);

    private:
        template <typename BufferContainer>
        void execFreeFromOtherThread(BufferContainer *bufferContainer);

    private:
        union Block
        {
            Offset _next;
            char _data[size];
        };

        Block *blocksArea();

        Block *offsetToBlock(Offset offset);
        Offset blockToOffset(Block *block);

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
        _next = blockToOffset(blocksArea());
        assert(_next);
        _allocated = _initialized = 0;

        _forFreeHolder._first = _forFreeHolder._last = 0;
        _forFreeHolder._amount = 0;
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
        assert(offsetToBlock(_next) >= blocksArea() && offsetToBlock(_next) <= blocksArea() + _blocksAmount);
        assert(_initialized <= _blocksAmount);
        assert(_allocated < _blocksAmount);

        if(unlikely(_allocated == _blocksAmount-1))
        {
            execFreeFromOtherThread(bufferContainer);

            if(unlikely(_allocated == _blocksAmount-1))
            {
                Block *block = offsetToBlock(_next);

                assert(_allocated == _initialized);
                _allocated = _blocksAmount;
                _next = blockToOffset(blocksArea() + _allocated);
                assert(_next);

                bufferContainer->template bufferMiddle2Full<size>(this);
                return block;
            }
        }

        Block *block = offsetToBlock(_next);

        if(unlikely(_allocated == _initialized))
        {
            const std::size_t protect = _blocksOffset + _initialized*sizeof(Block);
            const std::size_t inPage = protect % Config::_pageSize;
            if(unlikely(!inPage))
            {
                vm::protect(
                            reinterpret_cast<char *>(this) + protect,
                            Config::_pageSize,
                            true);
            }
            else if(unlikely(inPage > Config::_pageSize - sizeof(Block)))
            {
                vm::protect(
                            reinterpret_cast<char *>(this) + protect - (protect % Config::_pageSize) + Config::_pageSize,
                            Config::_pageSize,
                            true);
            }

            _next = blockToOffset(blocksArea() + _allocated + 1);
            assert(_next);
            _initialized++;
        }
        else
        {
            _next = block->_next;
            assert(_next);
        }

        _allocated++;

        if(unlikely(_allocated == 1))
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
        freeFromOtherThread(ptr, bufferContainer);
        //execFreeFromOtherThread(bufferContainer);
        return;

        assert(ptr >= blocksArea() && ptr < (blocksArea()+_blocksAmount*sizeof(Block)));
        assert(!(((char *)ptr - (char *)blocksArea()) % sizeof(Block)));

        Block *block = reinterpret_cast<Block *>(ptr);

        block->_next = _next;
        _next = blockToOffset(block);
        assert(_next);
        _allocated -= 1;

        if(unlikely(0 == _allocated))
        {
            bufferContainer->template bufferMiddle2Empty<size>(this);
        }
        else if(unlikely(_blocksAmount-1 == _allocated))
        {
            bufferContainer->template bufferFull2Middle<size>(this);
        }
    }

    namespace
    {
        std::atomic_flag af{false};
    }
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    template <typename BufferContainer>
    void SizedBuffer<size>::freeFromOtherThread(void *ptr, BufferContainer *bufferContainer)
    {
        Block *block = reinterpret_cast<Block *>(ptr);

        Counter was_amount =  __atomic_load_n(&_forFreeHolder._amount, __ATOMIC_RELAXED);
        for(;;)
        {
            if(was_amount)
            {
                block->_next = __atomic_load_n(&_forFreeHolder._first, __ATOMIC_RELAXED);
                {
                    if(af.test_and_set(std::memory_order_acquire))
                    {
                        _mm_pause();
                        was_amount = __atomic_load_n(&_forFreeHolder._amount, __ATOMIC_RELAXED);
                        continue;
                    }

                    Counter cur_amount = __atomic_load_n(&_forFreeHolder._amount, __ATOMIC_RELAXED);
                    if(was_amount != cur_amount)
                    {
                        af.clear(std::memory_order_relaxed);
                        was_amount = cur_amount;
                        continue;
                    }

                    _forFreeHolder._first = blockToOffset(block);
                    _forFreeHolder._amount = cur_amount+1;
                    af.clear(std::memory_order_release);
                }
                break;
            }
            else
            {
                block->_next = 0;
                {
                    if(af.test_and_set(std::memory_order_acquire))
                    {
                        _mm_pause();
                        was_amount = __atomic_load_n(&_forFreeHolder._amount, __ATOMIC_RELAXED);
                        continue;
                    }

                    Counter cur_amount = __atomic_load_n(&_forFreeHolder._amount, __ATOMIC_RELAXED);
                    if(was_amount != cur_amount)
                    {
                        af.clear(std::memory_order_relaxed);
                        was_amount = cur_amount;
                        continue;
                    }

                    _forFreeHolder._first = _forFreeHolder._last = blockToOffset(block);
                    _forFreeHolder._amount = 1;
                    af.clear(std::memory_order_release);
                }

                bufferContainer->template bufferAwaitFreeFromOtherThread<size>(this);
                break;
            }
        }


//        {
//            if(unlikely(_forFreeHolder._amount))
//            {
//                block->_next = _forFreeHolder._first;
//                _forFreeHolder._first = blockToOffset(block);
//                _forFreeHolder._amount++;
//            }
//            else
//            {
//                block->_next = 0;
//                _forFreeHolder._first = _forFreeHolder._last = blockToOffset(block);
//                _forFreeHolder._amount = 1;

//                bufferContainer->template bufferAwaitFreeFromOtherThread<size>(this);
//            }
//        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    template <typename BufferContainer>
    void SizedBuffer<size>::execFreeFromOtherThread(BufferContainer *bufferContainer)
    {
        ForFreeHolder forFreeHolder;

        Counter was_amount = __atomic_load_n(&_forFreeHolder._amount, __ATOMIC_RELAXED);

        if(!was_amount)
        {
            return;
        }

        while(af.test_and_set(std::memory_order_acquire))
        {
            _mm_pause();
        }

        {
            forFreeHolder = _forFreeHolder;
            _forFreeHolder._first = _forFreeHolder._last = 0;
            _forFreeHolder._amount = 0;
        }

        af.clear(std::memory_order_release);

        if(unlikely(forFreeHolder._amount))
        {
            bufferContainer->template bufferNotAwaitFreeFromOtherThread<size>(this);
        }

//        {
//            forFreeHolder = _forFreeHolder;
//            _forFreeHolder._first = _forFreeHolder._last = 0;
//            _forFreeHolder._amount = 0;

//            if(unlikely(forFreeHolder._amount))
//            {
//                bufferContainer->template bufferNotAwaitFreeFromOtherThread<size>(this);
//            }
//        }

        if(unlikely(forFreeHolder._amount))
        {
            Block *first = offsetToBlock(forFreeHolder._first);
            Block *last = offsetToBlock(forFreeHolder._last);

            assert(!last->_next);
            last->_next = _next;
            _next = blockToOffset(first);
            assert(_next);

            Counter wasAllocated = _allocated;
            _allocated -= forFreeHolder._amount;

            if(likely(_allocated))
            {
                if(unlikely(wasAllocated == _blocksAmount))
                {
                    bufferContainer->template bufferFull2Middle<size>(this);
                }
//                else
//                {
//                    bufferContainer->template bufferMiddle2Middle<size>(this);
//                }
            }
            else
            {
                if(unlikely(forFreeHolder._amount == _blocksAmount))
                {
                    bufferContainer->template bufferFull2Empty<size>(this);
                }
                else
                {
                    bufferContainer->template bufferMiddle2Empty<size>(this);
                }
            }
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
    typename SizedBuffer<size>::Block *SizedBuffer<size>::offsetToBlock(Offset offset)
    {
        return reinterpret_cast<Block *>(offset);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Offset SizedBuffer<size>::blockToOffset(Block *block)
    {
        return reinterpret_cast<Offset>(block);
    }

}}}

#endif
