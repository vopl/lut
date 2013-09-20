#ifndef _LUT_ASYNC_IMPL_CONTEXTENGINE_UCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CONTEXTENGINE_UCONTEXT_HPP_

#include <ucontext.h>
#include <cstdint>

namespace lut { namespace async { namespace impl
{
    class ContextEngine
        : private ucontext_t
    {
    public:
        ContextEngine();
        ContextEngine(void(* fn)(intptr_t), intptr_t arg, size_t stackSize);
        ~ContextEngine();

    protected:
        void *getStackBegin();
        void *getStackEnd();

    };
}}}

#endif
