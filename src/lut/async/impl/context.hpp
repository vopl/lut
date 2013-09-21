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

#include <functional>
#include <cstdint>

namespace lut { namespace async { namespace impl
{
    class Context
        : ContextEngine
    {
    public:
        Context();
        Context(const std::function<void()> &fn, size_t stackSize=1024*32);
        ~Context();

    private:
#if defined(USE_VALGRIND)
        int _valgrindStackId;
#endif
    };
}}}

#endif
