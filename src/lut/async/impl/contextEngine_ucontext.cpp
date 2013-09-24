#include "lut/async/stable.hpp"
#include "lut/async/impl/contextEngine_ucontext.hpp"

#include <cstdlib>

namespace lut { namespace async { namespace impl
{
    void ContextEngine::constructRoot()
    {
        if(getcontext(this))
        {
            std::cerr<<__FUNCTION__<<", getcontext failed"<<std::endl;
            std::terminate();
            return;
        }
    }

    void ContextEngine::destructRoot()
    {
        //empty ok
    }

#if PVOID_SIZE == INT_SIZE*2
    namespace
    {
        void coroProcForIntDiv2(int iproc1, int iproc2, int iarg1, int iarg2)
        {
            int64_t iproc = ((unsigned int)iproc1) | (((int64_t)iproc2)<<32);
            int64_t iarg = ((unsigned int)iarg1) | (((int64_t)iarg2)<<32);

            reinterpret_cast<void(*)(intptr_t)>(iproc)(iarg);
        }
    }
#endif
    void ContextEngine::constructCoro(size_t sizeWithStack, void(*f)(intptr_t), intptr_t arg)
    {
        if(getcontext(this))
        {
            std::cerr<<__FUNCTION__<<", getcontext failed"<<std::endl;
            std::terminate();
            return;
        }

        uc_link = NULL;
        uc_stack.ss_sp = reinterpret_cast<char*>(this) + sizeof(ContextEngine);
        uc_stack.ss_size = sizeWithStack - sizeof(ContextEngine);

#if PVOID_SIZE == INT_SIZE
        static_assert(sizeof(int) == sizeof(arg), "sizeof(int) == sizeof(task)");
        makecontext(this, (void(*)(void))f, 1, arg);
#elif PVOID_SIZE == INT_SIZE*2
        static_assert(sizeof(uint64_t) == sizeof(arg), "sizeof(uint64_t) == sizeof(task)");

        int64_t iarg = reinterpret_cast<int64_t>(arg);
        int iarg1 = (unsigned int)(iarg&0xffffffff);
        int iarg2 = (unsigned int)((iarg>>32)&0xffffffff);

        int64_t iproc = reinterpret_cast<int64_t>(f);
        int iproc1 = (unsigned int)(iproc&0xffffffff);
        int iproc2 = (unsigned int)((iproc>>32)&0xffffffff);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
        makecontext(this, (void (*)(void))&coroProcForIntDiv2, 4, iproc1, iproc2, iarg1, iarg2);
#pragma GCC diagnostic pop

#else
        error PVOID_SIZE not equal INT_SIZE or INT_SIZE*2
#endif

    }

    void ContextEngine::destructCoro()
    {
        //empry ok
    }

    void ContextEngine::switchTo(ContextEngine *to)
    {
        swapcontext(this, to);
    }
}}}
