#include "lut/mm/stable.hpp"
#include "lut/mm/impl/allocator.hpp"

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Allocator()
    {
        assert((std::uintptr_t)this == ((std::uintptr_t)this & (~(Config::_pageSize-1))));

        vm::protect(&_headerArea, sizeof(HeaderArea), true);
        new(&header()) Header;

        new(&stacksContainer()) StacksContainer;
        new(&buffersContainer()) BuffersContainer;

        createBufferForAlloc<0>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::~Allocator()
    {
        buffersContainer().~BuffersContainer();
        stacksContainer().~StacksContainer();

        header().~Header();
        vm::protect(&_headerArea, sizeof(HeaderArea), false);

        assert(this == _instance);
        _instance = nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator *Allocator::instance()
    {
        return _instance;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const lut::mm::Stack *Allocator::stackAlloc()
    {
        Stack *inst = stacksContainer().alloc();
        if(!inst)
        {
            return nullptr;
        }

        return Stack::impl2Face(inst);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Allocator::stackFree(const lut::mm::Stack *stack)
    {
        Stack *inst = Stack::face2Impl(stack);
        stacksContainer().free(inst);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Allocator::stackCompact(const lut::mm::Stack *stack)
    {
        Stack *inst = Stack::face2Impl(stack);
        inst->compact();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Allocator::vmAccessHandler(std::uintptr_t offset)
    {
        assert(offset < sizeof(Allocator));

        if(offset < offsetof(Allocator, _stacksContainerArea))
        {
            assert(!"must be already protected");
            return false;
        }

        if(offset < offsetof(Allocator, _buffersContainerArea))
        {
            return stacksContainer().vmAccessHandler(offset - offsetof(Allocator, _stacksContainerArea));
        }

        return buffersContainer().vmAccessHandler(offset - offsetof(Allocator, _buffersContainerArea));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Header::Header()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Header::~Header()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator *Allocator::_instance{nullptr};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Instantiator::Instantiator()
    {
        void *ptr = vm::alloc(sizeof(Allocator));
        Allocator::_instance = new(ptr) Allocator;
    }

    Allocator::Instantiator::~Instantiator()
    {
        Allocator::_instance->~Allocator();

        vm::free(Allocator::_instance, sizeof(Allocator));
        Allocator::_instance = nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Instantiator Allocator::_instantiator {};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Header &Allocator::header()
    {
        return *reinterpret_cast<Header *>(&_headerArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Allocator::relocateBufferDisposition(Buffer *buffer, Buffer *&bufferListSrc, Buffer *&bufferListDst)
    {
        if(buffer->_nextBufferInList)
        {
            buffer->_nextBufferInList->_prevBufferInList = buffer->_prevBufferInList;
        }
        if(buffer->_prevBufferInList)
        {
            buffer->_prevBufferInList->_nextBufferInList = buffer->_nextBufferInList;
        }
        if(bufferListSrc == buffer)
        {
            assert(!buffer->_prevBufferInList);
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
    Allocator::StacksContainer &Allocator::stacksContainer()
    {
        return *reinterpret_cast<StacksContainer *>(&_stacksContainerArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::BuffersContainer &Allocator::buffersContainer()
    {
        return *reinterpret_cast<BuffersContainer *>(&_buffersContainerArea);
    }

}}}
