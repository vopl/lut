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

        vm::protect(&_headerArea, sizeof(HeaderArea), true);
        new(&header()) Header;

        new(&stacksContainer()) StacksContainer;
        new(&buffersContainer()) BuffersContainer;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::~Thread()
    {
        buffersContainer().~BuffersContainer();
        stacksContainer().~StacksContainer();

        header().~Header();
        vm::protect(&_headerArea, sizeof(HeaderArea), false);

        assert(this == _instance);
        _instance = nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread *Thread::instance()
    {
        return _instance;
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
    void Thread::updateBufferDisposition(Buffer *buffer, BufferFullnessChange bfc, Header::BuffersBySize &buffersBySize)
    {
        if(BufferFullnessChange::Null == bfc)
        {
            return;
        }

        switch(bfc)
        {
        case BufferFullnessChange::Empty2Middle:
            return relocateBufferDisposition(buffer, buffersBySize._bufferListEmpty, buffersBySize._bufferListMiddle);
        case BufferFullnessChange::Middle2Empty:
            return relocateBufferDisposition(buffer, buffersBySize._bufferListMiddle, buffersBySize._bufferListEmpty);
        case BufferFullnessChange::Middle2Full:
            return relocateBufferDisposition(buffer, buffersBySize._bufferListMiddle, buffersBySize._bufferListFull);
        case BufferFullnessChange::Full2Middle:
            return relocateBufferDisposition(buffer, buffersBySize._bufferListFull, buffersBySize._bufferListMiddle);
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Thread::relocateBufferDisposition(Buffer *buffer, Buffer *&bufferListSrc, Buffer *&bufferListDst)
    {
        if(buffer->_nextBufferInList)
        {
            buffer->_nextBufferInList->_prevBufferInList = buffer->_prevBufferInList;
        }
        if(buffer->_prevBufferInList)
        {
            buffer->_prevBufferInList->_nextBufferInList = buffer->_prevBufferInList;
        }
        else
        {
            bufferListSrc = buffer->_nextBufferInList;
        }

        if(bufferListDst)
        {
            assert(!bufferListDst->_prevBufferInList);
            bufferListDst->_prevBufferInList = buffer;
            buffer->_nextBufferInList = bufferListDst;
            buffer->_prevBufferInList = nullptr;
            bufferListDst = buffer;
        }
        else
        {
            buffer->_nextBufferInList = nullptr;
            buffer->_prevBufferInList = nullptr;
            bufferListDst = buffer;
        }
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
