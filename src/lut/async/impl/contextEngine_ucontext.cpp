#include "lut/async/stable.hpp"
#include "lut/async/impl/contextEngine_ucontext.hpp"

#include <cstdlib>

namespace lut { namespace async { namespace impl
{
    ContextEngine::ContextEngine()
        : ucontext_t()
    {
        if(getcontext(this))
        {
            assert(!"getcontext failed");
            abort();
        }
    }

    ContextEngine::ContextEngine(size_t stackSize)
        : ucontext_t()
    {
        if(getcontext(this))
        {
            assert(!"getcontext failed");
            abort();
        }

        uc_link = nullptr;
        uc_stack.ss_sp = (char *)malloc(stackSize);
        uc_stack.ss_size = stackSize;

#if PVOID_SIZE == INT_SIZE
        int iarg1 = (unsigned int)(this);
        makecontext(this, (void (*)(void))&ContextEngine::s_contextProc, 1, iarg1);
#elif PVOID_SIZE == 2*INT_SIZE
        intptr_t fnArg = reinterpret_cast<intptr_t>(this);
        int iarg1 = (unsigned int)(fnArg&0xffffffff);
        int iarg2 = (unsigned int)((fnArg>>32)&0xffffffff);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
        makecontext(this, (void (*)(void))&ContextEngine::s_contextProc, 2, iarg1, iarg2);
#pragma GCC diagnostic pop

#else
#   error unknown int and pvoid sizes
#endif
    }

    ContextEngine::~ContextEngine()
    {
        if(haveStack())
        {
            free(uc_stack.ss_sp);
        }
    }

    bool ContextEngine::haveStack()
    {
        return uc_stack.ss_sp ? true : false;
    }

    void *ContextEngine::getStackBegin()
    {
        return uc_stack.ss_sp;
    }

    void *ContextEngine::getStackEnd()
    {
        return (char *)uc_stack.ss_sp + uc_stack.ss_size;
    }

    void ContextEngine::switchTo(ContextEngine *to)
    {
        if(swapcontext(this, to))
        {
            assert(!"swapcontext failed");
            abort();
        }
    }

#if PVOID_SIZE == INT_SIZE
    void ContextEngine::s_contextProc(int iarg1)
    {
        return reinterpret_cast<ContextEngine*>(iarg)->contextProc();
    }
#elif PVOID_SIZE == 2*INT_SIZE
    void ContextEngine::s_contextProc(int iarg1, int iarg2)
    {
        intptr_t callable = ((unsigned int)iarg1) | (((uint64_t)iarg2)<<32);

        return reinterpret_cast<ContextEngine*>(callable)->contextProc();
    }
#else
#   error unknown int and pvoid sizes
#endif

}}}
