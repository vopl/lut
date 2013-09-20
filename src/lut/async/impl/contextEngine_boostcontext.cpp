#include "lut/async/stable.hpp"
#include "lut/async/impl/contextEngine_boostcontext.hpp"

namespace lut { namespace async { namespace impl
{
    ContextEngine::ContextEngine()
        : _ctx()
    {
    }

    ContextEngine::ContextEngine(void(* fn)(intptr_t), intptr_t arg, size_t stackSize)
        : ContextEngine()
    {
    }

    ContextEngine::~ContextEngine()
    {
    }

    void *ContextEngine::getStackBegin()
    {
    }

    void *ContextEngine::getStackEnd()
    {
    }

}}}
