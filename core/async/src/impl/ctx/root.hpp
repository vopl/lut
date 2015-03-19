#pragma once

#include "config.h"

#if ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER
#   include "engine_winfiber.hpp"
#elif ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT
#   include "engine_ucontext.hpp"
#elif ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT
#   include "engine_boostcontext.hpp"
#else
#   error "unknown context engine"
#endif

#include <cstdint>

namespace dci { namespace async { namespace impl { namespace ctx
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
