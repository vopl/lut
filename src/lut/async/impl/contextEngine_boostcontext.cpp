#include "lut/async/stable.hpp"
#include "lut/async/impl/contextEngine_boostcontext.hpp"

namespace lut { namespace async { namespace impl
{
    ContextEngine::ContextEngine()
        : _ctx(new boost::context::fcontext_t)
        , _stackSize()
        , _stack()
        , _fn()
    {
    }

    ContextEngine::ContextEngine(const std::function<void()> &fn, size_t stackSize)
        : _ctx()
        , _stackSize(stackSize?stackSize:1)
        , _stack(new char [stackSize])
        , _fn(fn)
    {
        _ctx = boost::context::make_fcontext(_stack, _stackSize, &ContextEngine::s_contextProc);
    }

    ContextEngine::~ContextEngine()
    {
        if(_stackSize)
        {
            delete _stack;
        }
        else
        {
            delete _ctx;
        }
    }

    void *ContextEngine::getStackBegin()
    {
        return _stack;
    }

    void *ContextEngine::getStackEnd()
    {
        return _stack ? (_stack+_stackSize) : nullptr;
    }

    void ContextEngine::s_contextProc(intptr_t self)
    {
        return reinterpret_cast<ContextEngine*>(self)->_fn();
    }
}}}
