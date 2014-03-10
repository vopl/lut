#ifndef _LUT_MM_IMPL_BUFFERSCONTAINER_HPP_
#define _LUT_MM_IMPL_BUFFERSCONTAINER_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/impl/config.hpp"

namespace lut { namespace mm { namespace impl
{
    class BuffersContainer
    {
        char _pad[4096];

    };

}}}

#endif
