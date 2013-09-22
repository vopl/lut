#include "lut/async/stable.hpp"
#include "lut/async/impl/context.hpp"

#include <cstdlib>
#if defined(USE_VALGRIND)
#   include <valgrind.h>
#endif

namespace lut { namespace async { namespace impl
{
    Context::Context()
        : ContextEngine()
    {
    }

    Context::Context(size_t stackSize)
        : ContextEngine(stackSize)
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

    void Context::switchTo(Context *to)
    {
        ContextEngine::switchTo(to);
    }

    void Context::contextProc()
    {
        assert(!"never here");
        abort();
    }

}}}
