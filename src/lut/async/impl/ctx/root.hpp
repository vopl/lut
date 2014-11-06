#pragma once

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
    class Root
        : public Engine
    {
        Root &operator=(const Root &) = delete;
    public:
        Root();
        ~Root();
    };
}}}}
