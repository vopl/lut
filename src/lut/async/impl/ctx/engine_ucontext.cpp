#include "lut/async/stable.hpp"
#include "lut/async/impl/ctx/engine_ucontext.hpp"

#include <cstdlib>

namespace lut { namespace async { namespace impl { namespace ctx
{
    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructRoot()
    {
        if(getcontext(this))
        {
            std::cerr<<__FUNCTION__<<", getcontext failed"<<std::endl;
            std::terminate();
            return;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructRoot()
    {
        //empty ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    namespace
    {
        template<size_t ptrSizeDivIntSize = sizeof(void*)/sizeof(int)>
        struct ContextMaker;

        template<>
        struct ContextMaker<1>
        {
            static void make(ucontext_t *ucp, void(*f)(intptr_t), intptr_t arg)
            {
                return makecontext(ucp, (void(*)())f, 1, arg);
            }
        };

        template<>
        struct ContextMaker<2>
        {
            static void make(ucontext_t *ucp, void(*f)(intptr_t), intptr_t arg)
            {
                const uintptr_t intSize = (sizeof(int)*8);
                const uintptr_t intMask = (((uintptr_t)1) << intSize) - 1;

                uintptr_t iproc = reinterpret_cast<uintptr_t>(f);
                int iproc1 = (unsigned)((iproc>>intSize*0) & intMask);
                int iproc2 = (unsigned)((iproc>>intSize*1) & intMask);

                uintptr_t iarg = static_cast<uintptr_t>(arg);
                int iarg1 = (unsigned)((iarg>>intSize*0) & intMask);
                int iarg2 = (unsigned)((iarg>>intSize*1) & intMask);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
                return makecontext(ucp, (void(*)())&ContextMaker::procBridge, 4,
                                   iproc1, iproc2,
                                   iarg1, iarg2);
#pragma GCC diagnostic pop
            }

            static void procBridge(int iproc1, int iproc2, int iarg1, int iarg2)
            {
                const uintptr_t intSize = (sizeof(int)*8);

                uintptr_t iproc = ((unsigned)iproc1) | (((uintptr_t)iproc2)<<intSize);
                uintptr_t iarg = ((unsigned)iarg1) | (((uintptr_t)iarg2)<<intSize);

                reinterpret_cast<void(*)(intptr_t)>(iproc)(iarg);
            }
        };
    }

    void Engine::constructCoro(char *stackBegin, void(*f)(intptr_t), intptr_t arg)
    {
        if(getcontext(this))
        {
            std::cerr<<__FUNCTION__<<", getcontext failed"<<std::endl;
            std::terminate();
            return;
        }

        uc_link = NULL;
        uc_stack.ss_sp = stackBegin;
        uc_stack.ss_size = (char *)this - stackBegin;

        ContextMaker<>::make(this, f, arg);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructCoro()
    {
        //empry ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::switchTo(Engine *to)
    {
        swapcontext(this, to);
    }
}}}}
