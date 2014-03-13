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
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBlock<size>::SizedBlock()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBlock<size>::~SizedBlock()
    {
        assert(0);
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

}}}

#endif
