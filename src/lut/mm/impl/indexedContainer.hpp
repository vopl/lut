#ifndef _LUT_MM_IMPL_INDEXEDCONTAINER_HPP_
#define _LUT_MM_IMPL_INDEXEDCONTAINER_HPP_

#include "lut/mm/config.hpp"
#include "lut/mm/impl/bitIndex.hpp"
#include "lut/mm/impl/utils.hpp"

#include <type_traits>

namespace lut { namespace mm { namespace impl
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    class IndexedContainer
    {

        static_assert(sizeof(Block) == sizeof(Block)/Config::_pageSize*Config::_pageSize, "sizeof Block must be pageSize aligned");

    public:
        IndexedContainer();
        ~IndexedContainer();

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
    IndexedContainer<Block, amount>::IndexedContainer()
    {
        new (&index()) Index;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    IndexedContainer<Block, amount>::~IndexedContainer()
    {
        index().~Index();
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    Block *IndexedContainer<Block, amount>::alloc()
    {
        AddressInIndex addr = index().alloc();
        Block *ptr = blocks() + addr;
        new (ptr) Block;
        return ptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    void IndexedContainer<Block, amount>::free(Block *ptr)
    {
        AddressInIndex addr = ptr - blocks();
        index().free(addr);
        ptr->~Block();
    }









    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    template <typename DerivedBlock>
    DerivedBlock *IndexedContainer<Block, amount>::alloc()
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
    void IndexedContainer<Block, amount>::free(DerivedBlock *ptr)
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
    DerivedBlock *IndexedContainer<Block, amount>::blockByPointer(const void *ptr)
    {
        std::uintptr_t offset = reinterpret_cast<std::uintptr_t>(ptr) - reinterpret_cast<std::uintptr_t>(this);
        assert(offset < sizeof(IndexedContainer));

        if(offset < offsetof(IndexedContainer, _blocksArea))
        {
            return nullptr;
        }

        AddressInIndex blockAddr = (offset - offsetof(IndexedContainer, _blocksArea)) / sizeof(Block);

        if(index().isAllocated(blockAddr))
        {
            return static_cast<DerivedBlock *>(blocks() + blockAddr);
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    bool IndexedContainer<Block, amount>::vmAccessHandler(std::uintptr_t offset)
    {
        assert(offset < sizeof(IndexedContainer));

        if(offset < offsetof(IndexedContainer, _blocksArea))
        {
            return index().vmAccessHandler(offset - offsetof(IndexedContainer, _indexArea));
        }

        AddressInIndex blockAddr = (offset - offsetof(IndexedContainer, _blocksArea)) / sizeof(Block);

        if(index().isAllocated(blockAddr))
        {
            Block *block = blocks() + blockAddr;
            return block->vmAccessHandler(offset - offsetof(IndexedContainer, _blocksArea) - blockAddr * sizeof(Block));
        }

        assert(!"seg fault in unallocated block");
        return false;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    typename IndexedContainer<Block, amount>::Index &IndexedContainer<Block, amount>::index()
    {
        return *reinterpret_cast<Index *>(&_indexArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    Block *IndexedContainer<Block, amount>::blocks()
    {
        return reinterpret_cast<Block *>(&_blocksArea);
    }
}}}

#endif
