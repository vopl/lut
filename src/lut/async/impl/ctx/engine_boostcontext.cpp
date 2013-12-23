#include "lut/async/stable.hpp"
#include "lut/async/impl/ctx/engine_boostcontext.hpp"

#include <cstddef>

namespace lut { namespace async { namespace impl { namespace ctx
{
    ////////////////////////////////////////////////////////////////////////////////
    Engine::Engine()
    {

    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructRoot()
    {
        _coroArg = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructRoot()
    {
        //empty ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructCoro(char *stackBegin, void(*f)(intptr_t), intptr_t arg)
    {
        _coroArg = arg;

        char *stackEnd = (char *)this + sizeof(boost::context::fcontext_t);

        boost::context::fcontext_t *ctx = boost::context::make_fcontext(
                    stackEnd,
                    stackEnd - stackBegin,
                    f);

        assert(this == ctx);
        (void)ctx;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructCoro()
    {
        // empty ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::switchTo(Engine *to)
    {
        boost::context::jump_fcontext(this, to, to->_coroArg, false);
    }
}}}}
