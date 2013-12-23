#ifndef _LUT_ASYNC_IMPL_CTX_ENGINE_UCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CTX_ENGINE_UCONTEXT_HPP_

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

        void constructCoro(char *stackBegin, void(*f)(intptr_t), intptr_t arg);
        void destructCoro();

        void switchTo(Engine *to);
    };
}}}}

#endif
