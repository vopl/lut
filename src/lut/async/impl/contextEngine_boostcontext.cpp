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
        _ctx = boost::context::make_fcontext(sp, stackSize, &ContextEngine::s_contextProc);
    }

    ContextEngine::~ContextEngine()
    {
        if(haveStack())
        {
            free(_ctx->fc_stack.sp);
        }
        else
        {
            delete _ctx;
        }
    }

    bool ContextEngine::haveStack()
    {
        return _ctx->fc_stack.sp;
    }

    void *ContextEngine::getStackBegin()
    {
        return _ctx->fc_stack.sp;
    }

    void *ContextEngine::getStackEnd()
    {
        return (char*)_ctx->fc_stack.sp + _ctx->fc_stack.size;
    }

    void ContextEngine::activateFrom(ContextEngine *current)
    {
        boost::context::jump_fcontext(current->_ctx, _ctx, reinterpret_cast<intptr_t>(this));
    }

    void ContextEngine::s_contextProc(intptr_t callable)
    {
        return reinterpret_cast<ContextEngine*>(callable)->contextProc();
    }
}}}
