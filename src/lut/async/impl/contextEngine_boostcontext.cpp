#include "lut/async/stable.hpp"
#include "lut/async/impl/contextEngine_boostcontext.hpp"

namespace lut { namespace async { namespace impl
{
    ContextEngine::ContextEngine()
        : _ctx(new boost::context::fcontext_t)
    {
    }

    ContextEngine::ContextEngine(size_t stackSize)
        : _ctx()
    {
        void *sp = malloc(stackSize);
        _ctx = boost::context::make_fcontext((char *)sp+stackSize, stackSize, &ContextEngine::s_contextProc);
    }

    ContextEngine::~ContextEngine()
    {
        if(haveStack())
        {
            free((char *)_ctx->fc_stack.sp - _ctx->fc_stack.size);
        }
        else
        {
            delete _ctx;
        }
    }

    bool ContextEngine::haveStack()
    {
        return _ctx->fc_stack.sp ? true : false;
    }

    void *ContextEngine::getStackBegin()
    {
        return _ctx->fc_stack.sp;
    }

    void *ContextEngine::getStackEnd()
    {
        return (char*)_ctx->fc_stack.sp + _ctx->fc_stack.size;
    }

    void ContextEngine::switchTo(ContextEngine *to)
    {
        boost::context::jump_fcontext(_ctx, to->_ctx, reinterpret_cast<intptr_t>(to));
    }

    void ContextEngine::s_contextProc(intptr_t callable)
    {
        return reinterpret_cast<ContextEngine*>(callable)->contextProc();
    }
}}}
