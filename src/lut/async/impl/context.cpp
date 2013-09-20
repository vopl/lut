#include "lut/async/stable.hpp"
#include "lut/async/impl/context.hpp"

#if defined(USE_VALGRIND)
#   include <valgrind.h>
#endif

namespace lut { namespace async { namespace impl
{
    Context::Context()
        : ContextEngine()
    {
    }

    Context::Context(void(* fn)(intptr_t), intptr_t arg, size_t stackSize)
        : ContextEngine(fn, arg, stackSize)
    {
#if defined(USE_VALGRIND)
        _valgrindStackId = VALGRIND_STACK_REGISTER(getStackBegin(), getStackEnd());
#endif
    }

    Context::~Context()
    {
#if defined(USE_VALGRIND)
        VALGRIND_STACK_DEREGISTER(_valgrindStackId);
#endif
    }
}}}
