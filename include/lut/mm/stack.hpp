#ifndef _LUT_MM_STACK_HPP_
#define _LUT_MM_STACK_HPP_

#include "lut/mm/config.hpp"

namespace lut { namespace mm
{
    struct Stack
        : public ConfigMemory
        , public ConfigStack
    {
        char *_userspaceBegin;
        char *_userspaceMapped;
        char *_userspaceEnd;
    };
}}

#endif
