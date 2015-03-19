#include "engine_ucontext.hpp"
#include <dci/logger/logger.hpp>

#include <cstdlib>

namespace dci { namespace async { namespace impl { namespace ctx
{
    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructRoot()
    {
        if(getcontext(this))
        {
            LOGE(__FUNCTION__<<", getcontext failed");
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
        template<std::size_t ptrSizeDivIntSize = sizeof(void*)/sizeof(int)>
        struct ContextMaker;

        template<>
        struct ContextMaker<1>
        {
            static void make(ucontext_t *ucp, void(*f)(std::intptr_t), std::intptr_t arg)
            {
                return makecontext(ucp, (void(*)())f, 1, arg);
            }
        };

        template<>
        struct ContextMaker<2>
        {
            static void make(ucontext_t *ucp, void(*f)(std::intptr_t), std::intptr_t arg)
            {
                const std::uintptr_t intSize = (sizeof(int)*8);
                const std::uintptr_t intMask = (((std::uintptr_t)1) << intSize) - 1;

                std::uintptr_t iproc = reinterpret_cast<std::uintptr_t>(f);
                int iproc1 = (unsigned)((iproc>>intSize*0) & intMask);
                int iproc2 = (unsigned)((iproc>>intSize*1) & intMask);

                std::uintptr_t iarg = static_cast<std::uintptr_t>(arg);
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
                const std::uintptr_t intSize = (sizeof(int)*8);

                std::uintptr_t iproc = ((unsigned)iproc1) | (((std::uintptr_t)iproc2)<<intSize);
                std::uintptr_t iarg = ((unsigned)iarg1) | (((std::uintptr_t)iarg2)<<intSize);

                reinterpret_cast<void(*)(std::intptr_t)>(iproc)(iarg);
            }
        };
    }

    void Engine::constructCoro(char *stackPtr, std::size_t stackSize, void(*f)(std::intptr_t), std::intptr_t arg)
    {
        if(getcontext(this))
        {
            LOGE(__FUNCTION__<<", getcontext failed");
            std::terminate();
            return;
        }

        uc_link = NULL;
        uc_stack.ss_sp = stackPtr;
        uc_stack.ss_size = stackSize;

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
