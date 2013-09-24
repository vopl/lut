#ifndef _LUT_ASYNC_IMPL_CONTEXTENGINE_UCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CONTEXTENGINE_UCONTEXT_HPP_

#include "lut/async/config.h"

#include <ucontext.h>
#include <cstdint>

namespace lut { namespace async { namespace impl
{
    class ContextEngine
        : ucontext_t
    {
    public:
        void constructRoot();
        void destructRoot();

        void constructCoro(size_t sizeWithStack, void(*f)(intptr_t), intptr_t arg);
        void destructCoro();

        void switchTo(ContextEngine *to);
    };
}}}

#endif
