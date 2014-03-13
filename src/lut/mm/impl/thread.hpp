#ifndef _LUT_MM_IMPL_THREAD_HPP_
#define _LUT_MM_IMPL_THREAD_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/config.hpp"

#include "lut/mm/impl/indexedContainer.hpp"
#include "lut/mm/impl/stack.hpp"
#include "lut/mm/impl/block.hpp"
#include "lut/mm/impl/sizedBlock.hpp"

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
        template <std::size_t size> void *alloc();
        template <std::size_t size> void free(void *ptr);
        template <std::size_t size> void freeFromOtherThread(void *ptr);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
        static __thread Thread *_instance;

    private:
        struct Header
        {
            struct BlocksBySize
            {
                Block *_blockListFull;
                Block *_blockListMiddle;
                Block *_blockListEmpty;
            };

            BlocksBySize _blocksBySize[512];

            Header();
            ~Header();
        };

        using HeaderArea = std::aligned_storage<sizeof(Header), Config::_pageSize>::type;
        HeaderArea _headerArea;
        Header &header();

    private:
        void updateBlockDisposition(Block *block, BlockFullnessChange bfc, Header::BlocksBySize &blocksBySize);

    private:
        using StacksContainer = IndexedContainer<Stack, Config::_stacksAmount>;
        using StacksContainerArea = std::aligned_storage<sizeof(StacksContainer), Config::_pageSize>::type;

        StacksContainerArea _stacksContainerArea;
        StacksContainer &stacksContainer();

    private:
        using BlocksContainer = IndexedContainer<Block, Config::_blocksAmount>;
        using BlocksContainerArea = std::aligned_storage<sizeof(BlocksContainer), Config::_pageSize>::type;

        BlocksContainerArea _blocksContainerArea;
        BlocksContainer &blocksContainer();
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void *Thread::alloc()
    {
        if(0 == size || size > 512)
        {
            return ::malloc(size);
        }

        Header::BlocksBySize blocksBySize = header()._blocksBySize[size-1];

        SizedBlock<size> *sizedBlock;

        if(blocksBySize._blockListMiddle)
        {
            sizedBlock = static_cast<SizedBlock<size> *>(blocksBySize._blockListMiddle);
        }
        else
        {
            sizedBlock = blocksContainer().alloc<SizedBlock<size>>();
            if(!sizedBlock)
            {
                return nullptr;
            }

            blocksBySize._blockListEmpty = sizedBlock;
        }

        std::pair<void *, BlockFullnessChange> res = sizedBlock->alloc();
        assert(res.first);

        updateBlockDisposition(sizedBlock, res.second, blocksBySize);

        return res.first;
    }

    template <std::size_t size> void Thread::free(void *ptr)
    {
        if(0 == size || size > 512)
        {
            return ::free(ptr);
        }

        SizedBlock<size> *sizedBlock = blocksContainer().blockByPointer<SizedBlock<size>>(ptr);

        if(!sizedBlock)
        {
            return;
        }

        updateBlockDisposition(sizedBlock, sizedBlock->free(ptr), header()._blocksBySize[size-1]);
    }

    template <std::size_t size> void Thread::freeFromOtherThread(void *ptr)
    {
        if(0 == size || size > 512)
        {
            return ::free(ptr);
        }
        assert(0);
    }

}}}

#endif
