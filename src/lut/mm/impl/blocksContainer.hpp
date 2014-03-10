#ifndef _LUT_MM_IMPL_BLOCKSCONTAINER_HPP_
#define _LUT_MM_IMPL_BLOCKSCONTAINER_HPP_

#include "lut/mm/impl/config.hpp"
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
        Block *alloc();
        void free(Block *ptr);

    private:

        static const std::size_t _bitIndexLevelBittness = 5;
        static const std::size_t _bitIndexLevelDepth =
                std::conditional<
                    1 << (utils::bittness(amount) / _bitIndexLevelBittness) >= amount,
                    std::integral_constant<std::size_t, utils::bittness(amount) / _bitIndexLevelBittness>,
                    std::integral_constant<std::size_t, utils::bittness(amount) / _bitIndexLevelBittness + 1> >::type::value;

        BitIndex<_bitIndexLevelBittness, _bitIndexLevelDepth, amount> _bitIndex;


    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    Block *BlocksContainer<Block, amount>::alloc()
    {
        AddressInIndex addr = _bitIndex.alloc();
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Block, std::size_t amount>
    void BlocksContainer<Block, amount>::free(Block *ptr)
    {
        assert(0);
    }


}}}

#endif
