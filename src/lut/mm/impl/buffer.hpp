#ifndef _LUT_MM_IMPL_BUFFER_HPP_
#define _LUT_MM_IMPL_BUFFER_HPP_

#include "lut/mm/impl/config.hpp"

namespace lut { namespace mm { namespace impl
{
    class Buffer
    {
    public:
        Buffer();
        ~Buffer();

    private:
        char _stub[Config::_pageSize];
    };
}}}

#endif
