#pragma once

#include "lut/async/config.h"

#include <ucontext.h>
#include <cstdint>

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Engine
        : ucontext_t
    {
    public:
        void constructRoot();
        void destructRoot();

        void constructCoro(char *stackPtr, std::size_t stackSize, void(*f)(std::intptr_t), std::intptr_t arg);
        void destructCoro();

        void switchTo(Engine *to);
    };
}}}}
