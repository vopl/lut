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

#include <cstdint>

namespace lut { namespace async { namespace impl
{
    class Coro;
    class Context
    {
        Context &operator=(const Context &) = delete;
    public:
        Context();
        ~Context();

        void switchTo(Context *to);
        void switchTo(Coro *to);

    private:
        friend class Coro;
        ContextEngine _engine;
    };
}}}

#endif
