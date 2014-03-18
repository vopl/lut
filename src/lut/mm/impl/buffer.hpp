#ifndef _LUT_MM_IMPL_BUFFER_HPP_
#define _LUT_MM_IMPL_BUFFER_HPP_

#include "lut/mm/config.hpp"
#include <cstdint>
#include <type_traits>

namespace lut { namespace mm { namespace impl
{
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

        char * _areaAddress;

        using Offset = std::uint32_t;
        using Counter = std::uint32_t;

        Offset  _next;
        Counter _allocated;
        Counter _initialized;


        static const std::size_t _headerSize =
                sizeof(std::aligned_storage<sizeof(_prevBufferInList), alignof(Buffer *)>::type) +
                sizeof(std::aligned_storage<sizeof(_nextBufferInList), alignof(Buffer *)>::type) +
                sizeof(std::aligned_storage<sizeof(_areaAddress), alignof(char *)>::type) +
                sizeof(std::aligned_storage<sizeof(_next), alignof(Offset)>::type) +
                sizeof(std::aligned_storage<sizeof(_initialized), alignof(Counter)>::type) +
                sizeof(std::aligned_storage<sizeof(_allocated), alignof(Counter)>::type) +
                0;

        using Area = std::aligned_storage<Config::_pageSize * Config::_bufferPages - _headerSize, 1>::type;
        Area _area;
    };
}}}

#endif
