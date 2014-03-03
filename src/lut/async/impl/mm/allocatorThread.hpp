#ifndef _LUT_ASYNC_IMPL_MM_ALLOCATORTHREAD_HPP_
#define _LUT_ASYNC_IMPL_MM_ALLOCATORTHREAD_HPP_

#include "lut/async/impl/mm/stack.hpp"
#include "lut/async/impl/mm/config.hpp"

namespace lut { namespace async { namespace impl { namespace mm
{
    class AllocatorThread
    {
    private:
        AllocatorThread();

    public:
        static AllocatorThread &instance();
        ~AllocatorThread();

    public:
        const Stack *stackAlloc();
        bool stackFree(const Stack *stack);
        bool stackFreeFromOverThread(const Stack *stack);
        bool stackCompact(const Stack *stack);

    public:
        template <size_t size> void *bufferAlloc();
        template <size_t size> bool bufferFree(void *buffer);
        template <size_t size> bool bufferFreeFromOtherThread(void *buffer);

    private:
        bool vmAccessHandler(void *addr);

    private:
        static __thread AllocatorThread *_instance;

        char *_vspaceBegin;
        char *_vspaceEnd;

    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size> void *AllocatorThread::bufferAlloc()
    {
        assert(0);
    }

    template <size_t size> bool AllocatorThread::bufferFree(void *buffer)
    {
        assert(0);
    }

    template <size_t size> bool AllocatorThread::bufferFreeFromOtherThread(void *buffer)
    {
        assert(0);
    }


}}}}

#endif
