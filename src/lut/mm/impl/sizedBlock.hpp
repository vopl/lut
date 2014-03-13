#ifndef _LUT_MM_IMPL_SIZEDBLOCK_HPP_
#define _LUT_MM_IMPL_SIZEDBLOCK_HPP_

#include "lut/mm/impl/block.hpp"

namespace lut { namespace mm { namespace impl
{
    template <std::size_t size>
    class SizedBlock
        : public Block
    {
    public:
        SizedBlock();
        ~SizedBlock();

    public:
        std::pair<void *, BlockFullnessChange> alloc();
        BlockFullnessChange free(void *ptr);

    private:
        struct Header
        {
            int k;

        };
        Header &header();

        static const std::size_t _dataOffset = offsetof(Block, _area) + sizeof(std::aligned_storage<sizeof(Header), alignof(Header)>::type);
        static const std::size_t _dataSize = (sizeof(Block) - _dataOffset) / size;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBlock<size>::SizedBlock()
    {
        assert(!"protect");
        new (&header()) Header;


        header().k = 220;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBlock<size>::~SizedBlock()
    {
        header().~Header();
        assert(!"unprotect");
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    std::pair<void *, BlockFullnessChange> SizedBlock<size>::alloc()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    BlockFullnessChange SizedBlock<size>::free(void *ptr)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBlock<size>::Header &SizedBlock<size>::header()
    {
        return *reinterpret_cast<Header*>(_area);
    }

}}}

#endif
