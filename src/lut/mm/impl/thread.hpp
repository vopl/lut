#ifndef _LUT_MM_IMPL_THREAD_HPP_
#define _LUT_MM_IMPL_THREAD_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/config.hpp"

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

            BuffersBySize _buffersBySize[512];

            Header();
            ~Header();
        };

        using HeaderArea = std::aligned_storage<sizeof(Header), Config::_pageSize>::type;
        HeaderArea _headerArea;
        Header &header();

    private:
        void updateBufferDisposition(Buffer *buffer, BufferFullnessChange bfc, Header::BuffersBySize &buffersBySize);

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
        if(0 == size || size > 512)
        {
            return ::malloc(size);
        }

        Header::BuffersBySize buffersBySize = header()._buffersBySize[size-1];

        SizedBuffer<size> *sizedBuffer;

        if(buffersBySize._bufferListMiddle)
        {
            sizedBuffer = static_cast<SizedBuffer<size> *>(buffersBySize._bufferListMiddle);
        }
        else
        {
            sizedBuffer = buffersContainer().alloc<SizedBuffer<size>>();
            if(!sizedBuffer)
            {
                return nullptr;
            }

            buffersBySize._bufferListEmpty = sizedBuffer;
        }

        std::pair<void *, BufferFullnessChange> res = sizedBuffer->alloc();
        assert(res.first);

        updateBufferDisposition(sizedBuffer, res.second, buffersBySize);

        return res.first;
    }

    template <std::size_t size> void Thread::free(void *ptr)
    {
        if(0 == size || size > 512)
        {
            return ::free(ptr);
        }

        SizedBuffer<size> *sizedBuffer = buffersContainer().bufferByPointer<SizedBuffer<size>>(ptr);

        if(!sizedBuffer)
        {
            return;
        }

        updateBufferDisposition(sizedBuffer, sizedBuffer->free(ptr), header()._buffersBySize[size-1]);
    }

    template <std::size_t size> void Thread::freeFromOtherThread(void *ptr)
    {
        if(0 == size || size > 512)
        {
            return ::free(ptr);
        }
        assert(0);
    }

}}}

#endif
