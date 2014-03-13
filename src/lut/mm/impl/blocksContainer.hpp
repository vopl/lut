#ifndef _LUT_MM_IMPL_BLOCKSCONTAINER_HPP_
#define _LUT_MM_IMPL_BLOCKSCONTAINER_HPP_

#include "lut/mm/config.hpp"
#include "lut/mm/impl/bitIndex.hpp"
#include "lut/mm/impl/utils.hpp"

#include <type_traits>

namespace lut { namespace mm { namespace impl
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    class BlocksContainer
    {

        static_assert(sizeof(Block) == sizeof(Block)/Config::_pageSize*Config::_pageSize, "sizeof Block must be pageSize aligned");

    public:
        BlocksContainer();
        ~BlocksContainer();

        Block *alloc();
        void free(Block *ptr);

        template <typename DerivedBlock> DerivedBlock *alloc();
        template <typename DerivedBlock> void free(DerivedBlock *ptr);

        template <typename DerivedBlock> DerivedBlock *blockByPointer(const void *ptr);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
        using Index = BitIndex<amount>;
        using IndexArea = typename std::aligned_storage<sizeof(Index), Config::_pageSize>::type;

        IndexArea _indexArea;
        Index &index();

    private:
        using BlocksArea = typename std::aligned_storage<sizeof(Block)*amount, Config::_pageSize>::type;

        BlocksArea _blocksArea;
        Block *blocks();
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    BlocksContainer<Block, amount>::BlocksContainer()
    {
        new (&index()) Index;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    BlocksContainer<Block, amount>::~BlocksContainer()
    {
        index().~Index();
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    Block *BlocksContainer<Block, amount>::alloc()
    {
        AddressInIndex addr = index().alloc();
        Block *ptr = blocks() + addr;
        new (ptr) Block;
        return ptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    void BlocksContainer<Block, amount>::free(Block *ptr)
    {
        AddressInIndex addr = ptr - blocks();
        index().free(addr);
        ptr->~Block();
    }









    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    template <typename DerivedBlock>
    DerivedBlock *BlocksContainer<Block, amount>::alloc()
    {
        static_assert(sizeof(Block) == sizeof(DerivedBlock), "derived and base must have same layout");
        static_assert(std::is_base_of<Block, DerivedBlock>::value, "derivedBlock must be inherited from block");

        AddressInIndex addr = index().alloc();
        DerivedBlock *ptr = static_cast<DerivedBlock *>(blocks() + addr);
        new (ptr) DerivedBlock;
        return ptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    template <typename DerivedBlock>
    void BlocksContainer<Block, amount>::free(DerivedBlock *ptr)
    {
        static_assert(sizeof(Block) == sizeof(DerivedBlock), "derived and base must have same layout");
        static_assert(std::is_base_of<Block, DerivedBlock>::value, "derivedBlock must be inherited from block");

        AddressInIndex addr = ptr - blocks();
        index().free(addr);
        ptr->~DerivedBlock();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    template <typename DerivedBlock>
    DerivedBlock *BlocksContainer<Block, amount>::blockByPointer(const void *ptr)
    {
        std::uintptr_t offset = reinterpret_cast<std::uintptr_t>(ptr) - reinterpret_cast<std::uintptr_t>(this);
        assert(offset < sizeof(BlocksContainer));

        if(offset < offsetof(BlocksContainer, _blocksArea))
        {
            return nullptr;
        }

        AddressInIndex blockAddr = (offset - offsetof(BlocksContainer, _blocksArea)) / sizeof(Block);

        if(index().isAllocated(blockAddr))
        {
            return static_cast<DerivedBlock *>(blocks() + blockAddr);
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    bool BlocksContainer<Block, amount>::vmAccessHandler(std::uintptr_t offset)
    {
        assert(offset < sizeof(BlocksContainer));

        if(offset < offsetof(BlocksContainer, _blocksArea))
        {
            return index().vmAccessHandler(offset - offsetof(BlocksContainer, _indexArea));
        }

        AddressInIndex blockAddr = (offset - offsetof(BlocksContainer, _blocksArea)) / sizeof(Block);

        if(index().isAllocated(blockAddr))
        {
            Block *block = blocks() + blockAddr;
            return block->vmAccessHandler(offset - offsetof(BlocksContainer, _blocksArea) - blockAddr * sizeof(Block));
        }

        assert(!"seg fault in unallocated block");
        return false;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    typename BlocksContainer<Block, amount>::Index &BlocksContainer<Block, amount>::index()
    {
        return *reinterpret_cast<Index *>(&_indexArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    Block *BlocksContainer<Block, amount>::blocks()
    {
        return reinterpret_cast<Block *>(&_blocksArea);
    }
}}}

#endif
