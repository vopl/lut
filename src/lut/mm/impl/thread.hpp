#ifndef _LUT_MM_IMPL_THREAD_HPP_
#define _LUT_MM_IMPL_THREAD_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/config.hpp"

#include "lut/mm/impl/blocksContainer.hpp"
#include "lut/mm/impl/stack.hpp"
#include "lut/mm/impl/buffer.hpp"

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
        template <std::size_t size> bool bufferFree(void *buffer);
        template <std::size_t size> bool bufferFreeFromOtherThread(void *buffer);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
        static __thread Thread *_instance;

    private:
        struct Header
        {

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
        assert(0);
    }

    template <std::size_t size> bool Thread::bufferFree(void *buffer)
    {
        assert(0);
    }

    template <std::size_t size> bool Thread::bufferFreeFromOtherThread(void *buffer)
    {
        assert(0);
    }


}}}

#endif
