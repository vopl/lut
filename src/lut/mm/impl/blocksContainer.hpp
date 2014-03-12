#ifndef _LUT_MM_IMPL_BLOCKSCONTAINER_HPP_
#define _LUT_MM_IMPL_BLOCKSCONTAINER_HPP_

#include "lut/mm/config.hpp"
#include "lut/mm/impl/bitIndex.hpp"
#include "lut/mm/impl/utils.hpp"

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
