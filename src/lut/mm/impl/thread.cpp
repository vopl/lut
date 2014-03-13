#include "lut/mm/stable.hpp"
#include "lut/mm/impl/thread.hpp"

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::Thread()
    {
        assert((std::uintptr_t)this == ((std::uintptr_t)this & (~(Config::_pageSize-1))));

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
        Stack *inst = stacksContainer().alloc();
        if(!inst)
        {
            return nullptr;
        }

        return Stack::impl2Face(inst);
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
    bool Thread::vmAccessHandler(std::uintptr_t offset)
    {
        assert(offset < sizeof(Thread));

        if(offset < offsetof(Thread, _stacksContainerArea))
        {
            assert(!"must be already protected");
            return false;
        }

        if(offset < offsetof(Thread, _buffersContainerArea))
        {
            return stacksContainer().vmAccessHandler(offset - offsetof(Thread, _stacksContainerArea));
        }

        return buffersContainer().vmAccessHandler(offset - offsetof(Thread, _buffersContainerArea));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Thread::updateBufferDisposition(Buffer *buffer, BufferFullnessChange bfc)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::Header::Header()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::Header::~Header()
    {
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    __thread Thread *Thread::_instance{nullptr};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::Header &Thread::header()
    {
        return *reinterpret_cast<Header *>(&_headerArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::StacksContainer &Thread::stacksContainer()
    {
        return *reinterpret_cast<StacksContainer *>(&_stacksContainerArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::BuffersContainer &Thread::buffersContainer()
    {
        return *reinterpret_cast<BuffersContainer *>(&_buffersContainerArea);
    }

}}}
