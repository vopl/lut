#ifndef _LUT_MM_IMPL_STACK_HPP_
#define _LUT_MM_IMPL_STACK_HPP_

namespace lut { namespace mm { namespace impl
{
    struct Stack
    {
        char *_guardBound;
        char *_mappedBound;
        char *_userspaceBound;
    };
}}}

#endif
