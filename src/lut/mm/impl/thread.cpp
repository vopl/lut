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
        new(&blocksContainer()) BlocksContainer;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::~Thread()
    {
        blocksContainer().~BlocksContainer();
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

        if(offset < offsetof(Thread, _blocksContainerArea))
        {
            return stacksContainer().vmAccessHandler(offset - offsetof(Thread, _stacksContainerArea));
        }

        return blocksContainer().vmAccessHandler(offset - offsetof(Thread, _blocksContainerArea));
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
    void Thread::updateBlockDisposition(Block *block, BlockFullnessChange bfc, Header::BlocksBySize &blocksBySize)
    {
        Block **blockListSrc;
        Block **blockListDst;

        {
            switch(bfc)
            {
            case BlockFullnessChange::Empty2Middle:
                blockListSrc = &blocksBySize._blockListEmpty;
                blockListDst = &blocksBySize._blockListMiddle;
                break;
            case BlockFullnessChange::Middle2Empty:
                blockListSrc = &blocksBySize._blockListMiddle;
                blockListDst = &blocksBySize._blockListEmpty;
                break;
            case BlockFullnessChange::Middle2Full:
                blockListSrc = &blocksBySize._blockListMiddle;
                blockListDst = &blocksBySize._blockListFull;
                break;
            case BlockFullnessChange::Full2Middle:
                blockListSrc = &blocksBySize._blockListFull;
                blockListDst = &blocksBySize._blockListMiddle;
                break;
            }
        }

        Block *newSrcHead = block->_prevBlockInList ? nullptr : block->_nextBlockInList;

        if(block->_nextBlockInList)
        {
            block->_nextBlockInList->_prevBlockInList = block->_prevBlockInList;
        }
        if(block->_prevBlockInList)
        {
            block->_prevBlockInList->_nextBlockInList = block->_prevBlockInList;
        }

        if(newSrcHead)
        {
            (*blockListSrc) = newSrcHead;
        }

        if((*blockListDst))
        {
            assert(!(*blockListDst)->_prevBlockInList);
            (*blockListDst)->_prevBlockInList = block;
            block->_nextBlockInList = (*blockListDst);
            block->_prevBlockInList = nullptr;
            (*blockListDst) = block;
        }
        else
        {
            block->_nextBlockInList = nullptr;
            block->_prevBlockInList = nullptr;
            (*blockListDst) = block;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::StacksContainer &Thread::stacksContainer()
    {
        return *reinterpret_cast<StacksContainer *>(&_stacksContainerArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread::BlocksContainer &Thread::blocksContainer()
    {
        return *reinterpret_cast<BlocksContainer *>(&_blocksContainerArea);
    }

}}}
