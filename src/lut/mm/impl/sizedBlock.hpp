#ifndef _LUT_MM_IMPL_SIZEDBLOCK_HPP_
#define _LUT_MM_IMPL_SIZEDBLOCK_HPP_

#include "lut/mm/impl/block.hpp"
#include "lut/mm/impl/vm.hpp"

#include <cstdint>

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
//        using Offset = std::size_t;
//        static_assert(sizeof(Offset) <= size, "sizeof(Index) <= size");

//        union Block
//        {
//            Offset _next;
//            char _stub[size];
//        };

//        static_assert(sizeof(Block) == size, "sizeof(Block) == size");

//        static const Offset _badOffset = (Offset)-1;

        struct Header
        {
//            Offset _next;
//            Offset _allocated;
        };
        Header &header();

        static const std::size_t _dataOffset = _areaOffset + sizeof(typename std::aligned_storage<sizeof(Header), alignof(Header)>::type);
        static const std::size_t _dataSize = (sizeof(Block) - _dataOffset) / size;

    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBlock<size>::SizedBlock()
    {
        std::size_t protectedBound = sizeof(typename std::aligned_storage<_dataOffset, Config::_pageSize>::type);
        vm::protect(this, protectedBound, true);

        new (&header()) Header;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBlock<size>::~SizedBlock()
    {
        header().~Header();
        vm::protect(this, sizeof(Block), false);
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
