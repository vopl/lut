#ifndef _LUT_ASYNC_IMPL_CONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CONTEXT_HPP_

#include "lut/async/config.h"

#if ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER
#   include "lut/async/impl/contextEngine_winfiber.hpp"
#elif ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT
#   include "lut/async/impl/contextEngine_ucontext.hpp"
#elif ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT
#   include "lut/async/impl/contextEngine_boostcontext.hpp"
#else
#   error "unknown context engine"
#endif

#if defined(HAVE_VALGRIND)
#   define USE_VALGRIND
#endif

#include <cstdint>

namespace lut { namespace async { namespace impl
{
    class Context
        : private ContextEngine
    {
    public:
        Context();
        Context(size_t stackSize);
        ~Context();

    public:
        void switchTo(Context *to);

    private:
        virtual void contextProc() override;

    private:

#if defined(USE_VALGRIND)
        int _valgrindStackId;
#endif

    };
}}}

#endif
