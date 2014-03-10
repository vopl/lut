#ifndef _LUT_MM_STACK_HPP_
#define _LUT_MM_STACK_HPP_

namespace lut { namespace mm
{
    struct Stack
    {
        char *_guardBound;
        char *_mappedBound;
        char *_userspaceBound;
    };
}}

#endif
