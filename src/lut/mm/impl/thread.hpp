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

        Header::BuffersBySize &buffersBySize = header()._buffersBySize[size-1];

        SizedBuffer<size> *buffer;

        if(buffersBySize._bufferListMiddle)
        {
            buffer = static_cast<SizedBuffer<size> *>(buffersBySize._bufferListMiddle);
        }
        else if(buffersBySize._bufferListEmpty)
        {
            buffer = static_cast<SizedBuffer<size> *>(buffersBySize._bufferListEmpty);
        }
        else
        {
            buffer = buffersContainer().alloc<SizedBuffer<size>>();
            if(!buffer)
            {
                return nullptr;
            }

            buffersBySize._bufferListEmpty = buffer;
        }

        std::pair<void *, BufferFullnessChange> res = buffer->alloc();
        assert(res.first);

        //updateBufferDisposition(sizedBuffer, res.second, buffersBySize);
        if(BufferFullnessChange::Null == res.second)
        {
            return res.first;
        }

        switch(res.second)
        {
        case BufferFullnessChange::Empty2Middle:
            relocateBufferDisposition(buffer, buffersBySize._bufferListEmpty, buffersBySize._bufferListMiddle);
            break;
        case BufferFullnessChange::Middle2Full:
            relocateBufferDisposition(buffer, buffersBySize._bufferListMiddle, buffersBySize._bufferListFull);
            break;
        }

        return res.first;
    }

    template <std::size_t size> void Thread::free(void *ptr)
    {
        if(size > lut::mm::Allocator::_maxAllocatedBufferSize)
        {
            return ::free(ptr);
        }

        SizedBuffer<size> *buffer = buffersContainer().bufferByPointer<SizedBuffer<size>>(ptr);

        if(!buffer)
        {
            return;
        }

        //updateBufferDisposition(buffer, sizedBuffer->free(ptr), header()._buffersBySize[size-1]);
        BufferFullnessChange bfc = buffer->free(ptr);
        Header::BuffersBySize &buffersBySize = header()._buffersBySize[size-1];

        if(BufferFullnessChange::Null == bfc)
        {
            return;
        }

        switch(bfc)
        {
        case BufferFullnessChange::Middle2Empty:
            relocateBufferDisposition(buffer, buffersBySize._bufferListMiddle, buffersBySize._bufferListEmpty);
            break;
        case BufferFullnessChange::Full2Middle:
            relocateBufferDisposition(buffer, buffersBySize._bufferListFull, buffersBySize._bufferListMiddle);
            break;
        }
    }

    template <std::size_t size> void Thread::freeFromOtherThread(void *ptr)
    {
        if(size > lut::mm::Allocator::_maxAllocatedBufferSize)
        {
            return ::free(ptr);
        }
        assert(0);
    }

}}}

#endif
