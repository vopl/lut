#ifndef _LUT_MM_IMPL_BUFFER_HPP_
#define _LUT_MM_IMPL_BUFFER_HPP_

#include "lut/mm/config.hpp"
#include <cstdint>
#include <type_traits>

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    enum class BufferFullnessChange
    {
        Empty2Empty,
        Empty2Middle,
        Middle2Empty,
        Middle2Middle,
        Middle2Full,
        Full2Middle,
        Full2Full,
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

    protected:
        static const std::size_t _areaOffset =
                sizeof(std::aligned_storage<sizeof(_prevBufferInList), alignof(Buffer *)>::type) +
                sizeof(std::aligned_storage<sizeof(_nextBufferInList), alignof(Buffer *)>::type);

        char _area[Config::_pageSize * Config::_bufferPages - _areaOffset];
    };
}}}

#endif
