#ifndef _LUT_MM_IMPL_THREAD_HPP_
#define _LUT_MM_IMPL_THREAD_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/impl/config.hpp"

#include "lut/mm/impl/stacksContainer.hpp"
#include "lut/mm/impl/buffersContainer.hpp"

namespace lut { namespace mm { namespace impl
{
    class Thread
    {
    public:
        Thread();
        static Thread &instance();
        ~Thread();

    public:
        const Stack *stackAlloc();
        bool stackFree(const Stack *stack);
        bool stackFreeFromOverThread(const Stack *stack);
        bool stackCompact(const Stack *stack);

    public:
        template <size_t size> void *bufferAlloc();
        template <size_t size> bool bufferFree(void *buffer);
        template <size_t size> bool bufferFreeFromOtherThread(void *buffer);

    public:
        bool vmAccessHandler(void *addr);

    private:
        static __thread Thread *_instance;
        static const Config &_config;

        StacksContainer     _stacks;
        BuffersContainer    _buffers;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size> void *Thread::bufferAlloc()
    {
        assert(0);
    }

    template <size_t size> bool Thread::bufferFree(void *buffer)
    {
        assert(0);
    }

    template <size_t size> bool Thread::bufferFreeFromOtherThread(void *buffer)
    {
        assert(0);
    }


}}}

#endif
