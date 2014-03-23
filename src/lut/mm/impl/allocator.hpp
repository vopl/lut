#ifndef _LUT_MM_IMPL_ALLOCATOR_HPP_
#define _LUT_MM_IMPL_ALLOCATOR_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/config.hpp"
#include "lut/mm/allocator.hpp"

#include "lut/mm/impl/indexedContainer.hpp"
#include "lut/mm/impl/stack.hpp"
#include "lut/mm/impl/buffer.hpp"
#include "lut/mm/impl/sizedBuffer.hpp"
#include "lut/mm/impl/sizedBufferCalculator.hpp"

namespace lut { namespace mm { namespace impl
{
    class Allocator
    {
    public:
        Allocator();
        ~Allocator();

        static Allocator *instance();

    public:
        const lut::mm::Stack *stackAlloc();
        void stackFree(const lut::mm::Stack *stack);
        void stackCompact(const lut::mm::Stack *stack);

    public:
        template <std::size_t size> void *alloc();
        template <std::size_t size> void free(void *ptr);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    public:
        template <std::size_t size> void bufferEmpty2Middle(Buffer *buffer);
        template <std::size_t size> void bufferMiddle2Full(Buffer *buffer);
        template <std::size_t size> void bufferMiddle2Empty(Buffer *buffer);
        template <std::size_t size> void bufferFull2Middle(Buffer *buffer);
        template <std::size_t size> void bufferFull2Empty(Buffer *buffer);

    private:
        template <std::size_t size> void createBufferForAlloc();

    private:
        static Allocator *_instance;

    private:
        class Instantiator
        {
        public:
            Instantiator();
            ~Instantiator();
        };
        friend class Instantiator;

        static Instantiator _instantiator;


    private:
        struct Header
        {
            struct BuffersBySize
            {
                Buffer *_bufferListFull;
                Buffer *_bufferListMiddle;
                Buffer *_bufferListEmpty;

                Buffer *_bufferForAlloc;
            };

            BuffersBySize _buffersBySize[SizedBufferCalculator<lut::mm::Allocator::_maxAllocatedBufferSize>::_implAmount];

            Header();
            ~Header();
        };

        using HeaderArea = std::aligned_storage<sizeof(Header), Config::_pageSize>::type;
        HeaderArea _headerArea;
        Header &header();

    private:
        void relocateBufferDisposition(Buffer *buffer, Buffer *&bufferListSrc, Buffer *&bufferListDst);

    private:
        using StacksContainer = IndexedContainer<Stack, Config::_stacksAmount>;
        using StacksContainerArea = std::aligned_storage<sizeof(StacksContainer), Config::_pageSize>::type;

        StacksContainerArea _stacksContainerArea;
        StacksContainer &stacksContainer();

    private:
        using BuffersContainer = IndexedContainer<Buffer, Config::_buffersAmount>;
        using BuffersContainerArea = std::aligned_storage<sizeof(BuffersContainer), Config::_pageSize>::type;

        BuffersContainerArea _buffersContainerArea;
        BuffersContainer &buffersContainer();
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void *Allocator::alloc()
    {
        if(size > lut::mm::Allocator::_maxAllocatedBufferSize)
        {
            return ::malloc(size);
        }

//        Header::BuffersBySize buffersBySize = header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex];

//        if(likely(buffersBySize._bufferListMiddle))
//        {
//            return static_cast<SizedBuffer<size> *>(buffersBySize._bufferListMiddle)->alloc(this);
//        }
//        if(likely(buffersBySize._bufferListEmpty))
//        {
//            return static_cast<SizedBuffer<size> *>(buffersBySize._bufferListEmpty)->alloc(this);
//        }

        assert(header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex]._bufferForAlloc);
        return static_cast<SizedBuffer<size> *>(header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex]._bufferForAlloc)->alloc(this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void Allocator::free(void *ptr)
    {
        if(unlikely(size > lut::mm::Allocator::_maxAllocatedBufferSize))
        {
            return ::free(ptr);
        }

        SizedBuffer<size> *buffer = buffersContainer().bufferByPointer<SizedBuffer<size>>(ptr);

        assert(buffer);

        buffer->free(ptr, this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void Allocator::bufferEmpty2Middle(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex];
        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListEmpty,
                    buffersBySize._bufferListMiddle);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void Allocator::bufferMiddle2Full(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex];
        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListMiddle,
                    buffersBySize._bufferListFull);

        if(buffersBySize._bufferForAlloc == buffer)
        {
            if(buffersBySize._bufferListMiddle)
            {
                buffersBySize._bufferForAlloc = buffersBySize._bufferListMiddle;
            }
            else if(buffersBySize._bufferListEmpty)
            {
                assert(buffersBySize._bufferListEmpty->_allocated == 0);
                buffersBySize._bufferForAlloc = buffersBySize._bufferListEmpty;
            }
            else
            {
                buffer = buffersContainer().alloc<SizedBuffer<size>>();
                if(!buffer)
                {
                    assert(0);
                    return;
                }
                assert(buffer == buffersContainer().bufferByPointer<SizedBuffer<size>>(buffer));

                buffersBySize._bufferListEmpty = buffer;
                assert(buffersBySize._bufferListEmpty->_allocated == 0);
                buffersBySize._bufferForAlloc = buffersBySize._bufferListEmpty;
            }
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void Allocator::bufferMiddle2Empty(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex];

        while(buffersBySize._bufferListEmpty)
        {
            Buffer *empty = buffersBySize._bufferListEmpty;
            if(unlikely(empty == buffersBySize._bufferForAlloc))
            {
                buffersBySize._bufferForAlloc = buffer;
            }
            buffersBySize._bufferListEmpty = buffersBySize._bufferListEmpty->_nextBufferInList;
            buffersContainer().free(empty);
        }

        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListMiddle,
                    buffersBySize._bufferListEmpty);
        assert(buffersBySize._bufferListEmpty->_allocated == 0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void Allocator::bufferFull2Middle(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex];
        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListFull,
                    buffersBySize._bufferListMiddle);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void Allocator::bufferFull2Empty(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[SizedBufferCalculator<size>::_implSize2ImplIndex];

        while(buffersBySize._bufferListEmpty)
        {
            Buffer *empty = buffersBySize._bufferListEmpty;
            if(unlikely(empty == buffersBySize._bufferForAlloc))
            {
                buffersBySize._bufferForAlloc = buffer;
            }
            buffersBySize._bufferListEmpty = buffersBySize._bufferListEmpty->_nextBufferInList;
            buffersContainer().free(empty);
        }

        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListFull,
                    buffersBySize._bufferListEmpty);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t index>
    void Allocator::createBufferForAlloc()
    {
        Buffer *buffer = buffersContainer().alloc<SizedBuffer<SizedBufferCalculator<index>::_implIndex2ImplSize>>();
        if(!buffer)
        {
            assert(0);
            return;
        }
        assert(buffer == buffersContainer().bufferByPointer<SizedBuffer<SizedBufferCalculator<index>::_implIndex2ImplSize>>(buffer));

        Header::BuffersBySize &buffersBySize = header()._buffersBySize[index];
        buffersBySize._bufferListEmpty = buffer;
        buffersBySize._bufferForAlloc = buffer;
        assert(buffersBySize._bufferListEmpty->_allocated == 0);

        createBufferForAlloc<index+1>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    inline void Allocator::createBufferForAlloc<SizedBufferCalculator<lut::mm::Allocator::_maxAllocatedBufferSize>::_implAmount>()
    {
    }

}}}

#endif
