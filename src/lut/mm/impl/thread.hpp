#ifndef _LUT_MM_IMPL_THREAD_HPP_
#define _LUT_MM_IMPL_THREAD_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/config.hpp"
#include "lut/mm/allocator.hpp"

#include "lut/mm/impl/indexedContainer.hpp"
#include "lut/mm/impl/stack.hpp"
#include "lut/mm/impl/buffer.hpp"
#include "lut/mm/impl/sizedBuffer.hpp"

namespace lut { namespace mm { namespace impl
{
    class Thread
    {
    public:
        Thread();
        ~Thread();

        static Thread *instance();

    public:
        const lut::mm::Stack *stackAlloc();
        void stackFree(const lut::mm::Stack *stack);
        void stackFreeFromOverThread(const lut::mm::Stack *stack);
        void stackCompact(const lut::mm::Stack *stack);

    public:
        template <std::size_t size> void *alloc();
        template <std::size_t size> void free(void *ptr);
        template <std::size_t size> void freeFromOtherThread(void *ptr);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    public:
        template <std::size_t size> void bufferEmpty2Middle(Buffer *buffer);
        template <std::size_t size> void bufferMiddle2Full(Buffer *buffer);
        template <std::size_t size> void bufferMiddle2Empty(Buffer *buffer);
        template <std::size_t size> void bufferFull2Middle(Buffer *buffer);

    private:
        template <std::size_t size> void createBufferForAlloc();

    private:
        static __thread Thread *_instance;

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

            BuffersBySize _buffersBySize[lut::mm::Allocator::_maxAllocatedBufferSize+1];

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
    template <std::size_t size> void *Thread::alloc()
    {
        if(size > lut::mm::Allocator::_maxAllocatedBufferSize)
        {
            return ::malloc(size);
        }

        assert(header()._buffersBySize[size]._bufferForAlloc);
        return static_cast<SizedBuffer<size> *>(header()._buffersBySize[size]._bufferForAlloc)->alloc(this);
    }

    template <std::size_t size> void Thread::free(void *ptr)
    {
        if(size > lut::mm::Allocator::_maxAllocatedBufferSize)
        {
            return ::free(ptr);
        }

        SizedBuffer<size> *buffer = buffersContainer().bufferByPointer<SizedBuffer<size>>(ptr);

        assert(buffer);

        buffer->free(ptr, this);
    }

    template <std::size_t size> void Thread::freeFromOtherThread(void *ptr)
    {
        if(size > lut::mm::Allocator::_maxAllocatedBufferSize)
        {
            return ::free(ptr);
        }
        assert(0);
    }

    template <std::size_t size> void Thread::bufferEmpty2Middle(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[size];
        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListEmpty,
                    buffersBySize._bufferListMiddle);
    }

    template <std::size_t size> void Thread::bufferMiddle2Full(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[size];
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

                buffersBySize._bufferListEmpty = buffer;
                buffersBySize._bufferForAlloc = buffersBySize._bufferListEmpty;
            }
        }
    }

    template <std::size_t size> void Thread::bufferMiddle2Empty(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[size];
        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListMiddle,
                    buffersBySize._bufferListEmpty);
    }

    template <std::size_t size> void Thread::bufferFull2Middle(Buffer *buffer)
    {
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[size];
        relocateBufferDisposition(
                    buffer,
                    buffersBySize._bufferListFull,
                    buffersBySize._bufferListMiddle);
    }

    template <std::size_t size>
    void Thread::createBufferForAlloc()
    {
        Buffer *buffer = buffersContainer().alloc<SizedBuffer<size>>();
        if(!buffer)
        {
            assert(0);
            return;
        }

        Header::BuffersBySize &buffersBySize = header()._buffersBySize[size];
        buffersBySize._bufferListEmpty = buffer;
        buffersBySize._bufferForAlloc = buffersBySize._bufferListEmpty;

        createBufferForAlloc<size+1>();
    }

    template <>
    inline void Thread::createBufferForAlloc<lut::mm::Allocator::_maxAllocatedBufferSize>()
    {
    }

}}}

#endif
