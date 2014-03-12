#ifndef _LUT_MM_IMPL_BUFFER_HPP_
#define _LUT_MM_IMPL_BUFFER_HPP_

#include "lut/mm/config.hpp"
#include <cstdint>

namespace lut { namespace mm { namespace impl
{
    class Buffer
    {
    public:
        Buffer();
        ~Buffer();

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
        char _stub[Config::_pageSize];
    };
}}}

#endif
