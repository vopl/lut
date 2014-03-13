#ifndef _LUT_MM_IMPL_SIZEDBUFFER_HPP_
#define _LUT_MM_IMPL_SIZEDBUFFER_HPP_

#include "lut/mm/impl/buffer.hpp"

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
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::SizedBuffer()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size>
    SizedBuffer<size>::~SizedBuffer()
    {
        assert(0);
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

}}}

#endif
