#include "lut/async/stable.hpp"
#include "lut/async/impl/contextEngine_boostcontext.hpp"

#include <cstddef>

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    ContextEngine::ContextEngine()
    {

    }

    ////////////////////////////////////////////////////////////////////////////////
    void ContextEngine::constructRoot()
    {
        _coroArg = 0;
        _ctx = reinterpret_cast<boost::context::fcontext_t *>(&_buffer);
        new(_ctx) boost::context::fcontext_t;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void ContextEngine::destructRoot()
    {
        //empty ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    void ContextEngine::constructCoro(size_t sizeWithStack, void(*f)(intptr_t), intptr_t arg)
    {
        _coroArg = arg;

        size_t stackSize = sizeWithStack-offsetof(ContextEngine, _buffer);

        _ctx = boost::context::make_fcontext(
                    (char *)&_buffer+stackSize,
                    stackSize,
                    f);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void ContextEngine::destructCoro()
    {
        // empty ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    void ContextEngine::switchTo(ContextEngine *to)
    {
        boost::context::jump_fcontext(_ctx, to->_ctx, to->_coroArg, false);
    }
}}}
