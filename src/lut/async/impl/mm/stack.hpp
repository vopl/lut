#ifndef _LUT_ASYNC_IMPL_MM_STACK_HPP_
#define _LUT_ASYNC_IMPL_MM_STACK_HPP_

namespace lut { namespace async { namespace impl { namespace mm
{
    struct Stack
    {
        char *_guardBound;
        char *_mappedBound;
        char *_userspaceBound;
    };
}}}}

#endif
