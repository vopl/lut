#include "lut/async/stable.hpp"
#include "lut/async/impl/contextEngine_ucontext.hpp"
#include "lut/async/config.h"

#include <cstdlib>

namespace lut { namespace async { namespace impl
{
    ContextEngine::ContextEngine()
        : ucontext_t()
    {
        if(getcontext(this))
        {
            assert(!"handle errors");
        }
    }

    namespace
    {
#if PVOID_SIZE == INT_SIZE
        void workerProcWrapper(int iarg1, int iarg2)
        {
            typedef void(* Tfn)(intptr_t);
            Tfn fn((Tfn)iarg1);
            return fn(iarg2);
        }
#elif PVOID_SIZE == 2*INT_SIZE
        void workerProcWrapper(int iarg1, int iarg2, int iarg3, int iarg4)
        {
            intptr_t fnArg = ((unsigned int)iarg1) | (((uint64_t)iarg2)<<32);
            intptr_t argArg = ((unsigned int)iarg3) | (((uint64_t)iarg4)<<32);

            typedef void(* Tfn)(intptr_t);
            Tfn fn((Tfn)fnArg);
            return fn(argArg);
        }
#else
#   error unknown int and pvoid sizes
#endif
    }

    ContextEngine::ContextEngine(void(* fn)(intptr_t), intptr_t arg, size_t stackSize)
        : ContextEngine()
    {
        static_assert(sizeof(fn) == sizeof(intptr_t), "sizeof(fn) == sizeof(intptr_t)");

        uc_link = nullptr;
        uc_stack.ss_sp = (char *)malloc(stackSize);
        uc_stack.ss_size = stackSize;

#if PVOID_SIZE == INT_SIZE
        int iarg1 = (unsigned int)(fnArg);
        int iarg2 = (unsigned int)(arg);
        makecontext(this, (void (*)(void))workerProcWrapper, 2, iarg1, iarg2);
#elif PVOID_SIZE == 2*INT_SIZE
        intptr_t fnArg = reinterpret_cast<uint64_t>(fn);
        int iarg1 = (unsigned int)(fnArg&0xffffffff);
        int iarg2 = (unsigned int)((fnArg>>32)&0xffffffff);
        int iarg3 = (unsigned int)(arg&0xffffffff);
        int iarg4 = (unsigned int)((arg>>32)&0xffffffff);
        makecontext(this, (void (*)(void))workerProcWrapper, 4, iarg1, iarg2, iarg3, iarg4);
#else
#   error unknown int and pvoid sizes
#endif
    }

    ContextEngine::~ContextEngine()
    {
        if(uc_stack.ss_sp)
        {
            free(uc_stack.ss_sp);
        }
    }

    void *ContextEngine::getStackBegin()
    {
        return uc_stack.ss_sp;
    }

    void *ContextEngine::getStackEnd()
    {
        return (char *)uc_stack.ss_sp + uc_stack.ss_size;
    }

}}}
