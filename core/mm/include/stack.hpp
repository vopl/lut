#pragma once

#include "config.hpp"

namespace dci { namespace mm
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
