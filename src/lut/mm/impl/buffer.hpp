#ifndef _LUT_MM_IMPL_BUFFER_HPP_
#define _LUT_MM_IMPL_BUFFER_HPP_

#include "lut/mm/config.hpp"
#include <cstdint>

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    enum class BufferFullnessChange
    {
        Empty2Middle,
        Middle2Empty,
        Middle2Full,
        Full2Middle,
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Buffer
    {
    public:
        Buffer();
        ~Buffer();

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    public:
        Buffer *_prevBufferInList;
        Buffer *_nextBufferInList;

    private:
        char _stub[Config::_pageSize-8-8];
    };
}}}

#endif
