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
        static const std::size_t _cacheLineSize = 64;
    public:

        char * _areaAddress;

        using Offset = std::uintptr_t;
        using Counter = std::uint32_t;

        Offset  _next;
        Counter _allocated;
        Counter _initialized;

        struct ForFreeHolder
        {
            Offset _first;
            Offset _last;
            Counter _amount;
        };

        alignas(_cacheLineSize) ForFreeHolder _forFreeHolder;

        static const std::size_t _headerSize = _cacheLineSize * 2;

        using Area = std::aligned_storage<Config::_pageSize * Config::_bufferPages - _headerSize, 1>::type;
        Area _area;
    };
}}}

#endif
