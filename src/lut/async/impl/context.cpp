#include "lut/async/stable.hpp"
#include "lut/async/impl/context.hpp"

#include <cstdlib>
#if defined(USE_VALGRIND)
#   include <valgrind.h>
#endif

namespace lut { namespace async { namespace impl
{
    thread_local Context *Context::_currentInThread(nullptr);

    Context::Context()
        : ContextEngine()
    {
        assert(!_currentInThread);
        _currentInThread = this;
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
        if(!haveStack())
        {
            assert(this == _currentInThread);
            _currentInThread = nullptr;
        }

#if defined(USE_VALGRIND)
        VALGRIND_STACK_DEREGISTER(_valgrindStackId);
#endif
    }

    void Context::activate()
    {
        Context *current = _currentInThread;

        assert(current);
        assert(this == current);

        _currentInThread = this;
        activateFrom(current);

        assert(this == current);
    }

    void Context::contextProc()
    {
        assert(!"never here");
        abort();
    }

}}}
