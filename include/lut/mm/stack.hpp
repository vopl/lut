#pragma once

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
