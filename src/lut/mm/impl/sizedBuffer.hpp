#ifndef _LUT_MM_IMPL_SIZEDBUFFER_HPP_
#define _LUT_MM_IMPL_SIZEDBUFFER_HPP_

#include "lut/mm/impl/buffer.hpp"
#include "lut/mm/impl/vm.hpp"

#include <cstdint>

namespace lut { namespace mm { namespace impl
{
    template <std::size_t size>
    class SizedBuffer
        : public Buffer
    {
    public:
        SizedBuffer();
        ~SizedBuffer();

    public:
        std::pair<void *, BufferFullnessChange> alloc();
        BufferFullnessChange free(void *ptr);

    private:
        using Offset = std::uint32_t;

        struct Block
        {
            char _stub[size < sizeof(Offset) ? sizeof(Offset) : size];

            Offset &next();
        };

        static const Offset _badOffset = (Offset)-1;

        struct Header
        {
            Offset _next;
            Offset _allocated;
        };
        Header &header();

        static const std::size_t _dataOffset = _areaOffset + sizeof(typename std::aligned_storage<sizeof(Header), alignof(Header)>::type);
        static const std::size_t _dataSize = (sizeof(Buffer) - _dataOffset) / size;

    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::SizedBuffer()
    {
        std::size_t protectedBound = sizeof(typename std::aligned_storage<_dataOffset, Config::_pageSize>::type);
        vm::protect(this, protectedBound, true);

        new (&header()) Header;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::~SizedBuffer()
    {
        header().~Header();
        vm::protect(this, sizeof(Buffer), false);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    std::pair<void *, BufferFullnessChange> SizedBuffer<size>::alloc()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    BufferFullnessChange SizedBuffer<size>::free(void *ptr)
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    typename SizedBuffer<size>::Header &SizedBuffer<size>::header()
    {
        return *reinterpret_cast<Header*>(_area);
    }

}}}

#endif
