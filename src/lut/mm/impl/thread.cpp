#include "lut/async/stable.hpp"
#include "lut/mm/impl/thread.hpp"

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::Thread()
    {
        assert((uintptr_t)this == ((uintptr_t)this & (~(Config::_pageSize-1))));

        assert(!_instance);
        _instance = this;

        new(&header()) Header;
        new(&stacksContainer()) StacksContainer;
        new(&buffersContainer()) BuffersContainer;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::~Thread()
    {
        header().~Header();
        stacksContainer().~StacksContainer();
        buffersContainer().~BuffersContainer();

        assert(this == _instance);
        _instance = nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread &Thread::instance()
    {
        return *_instance;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const lut::mm::Stack *Thread::stackAlloc()
    {
        void *ptr = stacksContainer().alloc();
        if(!ptr)
        {
            return nullptr;
        }

        return Stack::impl2Face(new(ptr) Stack);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Thread::stackFree(const lut::mm::Stack *stack)
    {
        Stack *inst = Stack::face2Impl(stack);
        stacksContainer().free(inst);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Thread::stackFreeFromOverThread(const lut::mm::Stack *stack)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Thread::stackCompact(const lut::mm::Stack *stack)
    {
        Stack *inst = Stack::face2Impl(stack);
        inst->compact();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Thread::vmAccessHandler(void *addr)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    __thread Thread *Thread::_instance{nullptr};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::Header &Thread::header()
    {
        union
        {
            HeaderArea *_ha;
            Header *_h;
        } u;
        u._ha = &_headerArea;
        return *u._h;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::StacksContainer &Thread::stacksContainer()
    {
        union
        {
            StacksContainerArea *_ha;
            StacksContainer *_h;
        } u;
        u._ha = &_stacksContainerArea;
        return *u._h;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::BuffersContainer &Thread::buffersContainer()
    {
        union
        {
            BuffersContainerArea *_ha;
            BuffersContainer *_h;
        } u;
        u._ha = &_buffersContainerArea;
        return *u._h;
    }

}}}
