#ifndef _LUT_ASYNC_IMPL_CONTEXTENGINE_UCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CONTEXTENGINE_UCONTEXT_HPP_

#include "lut/async/config.h"

#include <ucontext.h>
#include <cstdint>

namespace lut { namespace async { namespace impl
{
    class ContextEngine
        : private ucontext_t
    {
    public:
        ContextEngine();
        ContextEngine(size_t stackSize);
        virtual ~ContextEngine();

    protected:
        void *getStackBegin();
        void *getStackEnd();

    private:
#if PVOID_SIZE == INT_SIZE
        void s_contextProc(int iarg1);
#elif PVOID_SIZE == 2*INT_SIZE
        void s_contextProc(int iarg1, int iarg2);
#else
#   error unknown int and pvoid sizes
#endif

    private:
        virtual void contextProc() = 0;

    };
}}}

#endif
