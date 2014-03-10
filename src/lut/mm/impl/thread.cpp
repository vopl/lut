#include "lut/async/stable.hpp"
#include "lut/mm/impl/thread.hpp"

namespace lut { namespace mm { namespace impl
{
    Thread::Thread()
    {
        assert((uintptr_t)this == ((uintptr_t)this & (~(Config::_pageSize-1))));

        assert(!_instance);
        _instance = this;
    }

    Thread &Thread::instance()
    {
        return *_instance;
    }

    Thread::~Thread()
    {
        assert(this == _instance);
        _instance = nullptr;
    }

    const Stack *Thread::stackAlloc()
    {
        assert(0);
    }

    bool Thread::stackFree(const Stack *stack)
    {
        assert(0);
    }

    bool Thread::stackFreeFromOverThread(const Stack *stack)
    {
        assert(0);
    }

    bool Thread::stackCompact(const Stack *stack)
    {
        assert(0);
    }

    bool Thread::vmAccessHandler(void *addr)
    {
        assert(0);
    }

    __thread Thread *Thread::_instance{nullptr};
    const Config &Thread::_config{Config::instance()};


}}}
