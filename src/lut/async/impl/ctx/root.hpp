#ifndef _LUT_ASYNC_IMPL_CTX_ROOT_HPP_
#define _LUT_ASYNC_IMPL_CTX_ROOT_HPP_

#include "lut/async/config.h"

#if ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER
#   include "lut/async/impl/ctx/engine_winfiber.hpp"
#elif ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT
#   include "lut/async/impl/ctx/engine_ucontext.hpp"
#elif ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT
#   include "lut/async/impl/ctx/engine_boostcontext.hpp"
#else
#   error "unknown context engine"
#endif

#include <cstdint>

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Coro;
    class Root
    {
        Root &operator=(const Root &) = delete;
    public:
        Root();
        ~Root();

        void switchTo(Root *to);
        void switchTo(Coro *to);

    private:
        friend class Coro;
        Engine _engine;
    };
}}}}

#endif
