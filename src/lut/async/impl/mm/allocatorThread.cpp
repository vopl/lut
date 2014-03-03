#include "lut/async/stable.hpp"
#include "lut/async/impl/mm/allocatorThread.hpp"

namespace lut { namespace async { namespace impl { namespace mm
{
    AllocatorThread::AllocatorThread()
    {
        assert(0);
    }

    AllocatorThread &AllocatorThread::instance()
    {
        assert(0);
    }

    AllocatorThread::~AllocatorThread()
    {
        assert(0);
    }

    const Stack *AllocatorThread::stackAlloc()
    {
        assert(0);
    }

    bool AllocatorThread::stackFree(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorThread::stackFreeFromOverThread(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorThread::stackCompact(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorThread::vmAccessHandler(void *addr)
    {
        assert(0);
    }


}}}}
