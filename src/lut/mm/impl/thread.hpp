#ifndef _LUT_MM_IMPL_THREAD_HPP_
#define _LUT_MM_IMPL_THREAD_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/config.hpp"

#include "lut/mm/impl/blocksContainer.hpp"
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

        static Thread &instance();

    public:
        const lut::mm::Stack *stackAlloc();
        void stackFree(const lut::mm::Stack *stack);
        void stackFreeFromOverThread(const lut::mm::Stack *stack);
        void stackCompact(const lut::mm::Stack *stack);

    public:
        template <std::size_t size> void *bufferAlloc();
        template <std::size_t size> void bufferFree(void *buffer);
        template <std::size_t size> void bufferFreeFromOtherThread(void *buffer);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
        void updateBufferDisposition(Buffer *buffer, BufferFullnessChange bfc);

    private:
        static __thread Thread *_instance;

    private:
        struct Header
        {
            struct BuffersBySize
            {
                Buffer *_bufferListFull;
                Buffer *_bufferListNotFull;
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
        using StacksContainer = BlocksContainer<Stack, Config::_stacksAmount>;
        using StacksContainerArea = std::aligned_storage<sizeof(StacksContainer), Config::_pageSize>::type;

        StacksContainerArea _stacksContainerArea;
        StacksContainer &stacksContainer();

    private:
        using BuffersContainer = BlocksContainer<Buffer, Config::_buffersAmount>;
        using BuffersContainerArea = std::aligned_storage<sizeof(BuffersContainer), Config::_pageSize>::type;

        BuffersContainerArea _buffersContainerArea;
        BuffersContainer &buffersContainer();
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void *Thread::bufferAlloc()
    {
        if(0 == size || size > 512)
        {
            return ::malloc(size);
        }

        Header::BuffersBySize buffersBySize = header()._buffersBySize[size-1];

        SizedBuffer<size> *sizedBuffer;

        if(buffersBySize._bufferListNotFull)
        {
            sizedBuffer = static_cast<SizedBuffer<size> *>(buffersBySize._bufferListNotFull);
        }
        else
        {
            sizedBuffer = buffersContainer().alloc<SizedBuffer<size>>();
            if(!sizedBuffer)
            {
                return nullptr;
            }

            buffersBySize._bufferListNotFull = sizedBuffer;
        }

        std::pair<void *, BufferFullnessChange> res = sizedBuffer->alloc();
        assert(res.first);

        updateBufferDisposition(sizedBuffer, res.second);

        return res.first;
    }

    template <std::size_t size> void Thread::bufferFree(void *buffer)
    {
        if(0 == size || size > 512)
        {
            return ::free(buffer);
        }

        SizedBuffer<size> *sizedBuffer = buffersContainer().blockByPointer<SizedBuffer<size>>(buffer);

        if(!sizedBuffer)
        {
            return;
        }

        updateBufferDisposition(sizedBuffer, sizedBuffer->free(buffer));
    }

    template <std::size_t size> void Thread::bufferFreeFromOtherThread(void *buffer)
    {
        if(0 == size || size > 512)
        {
            return ::free(buffer);
        }
        assert(0);
    }


}}}

#endif
