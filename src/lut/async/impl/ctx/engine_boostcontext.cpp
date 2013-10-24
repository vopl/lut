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
        _ctx = reinterpret_cast<boost::context::fcontext_t *>(&_buffer);
        new(_ctx) boost::context::fcontext_t;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructRoot()
    {
        //empty ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructCoro(size_t sizeWithStack, void(*f)(intptr_t), intptr_t arg)
    {
        _coroArg = arg;

        size_t stackSize = sizeWithStack-offsetof(Engine, _buffer);

        _ctx = boost::context::make_fcontext(
                    (char *)&_buffer+stackSize,
                    stackSize,
                    f);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructCoro()
    {
        // empty ok
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::switchTo(Engine *to)
    {
        boost::context::jump_fcontext(_ctx, to->_ctx, to->_coroArg, false);
    }
}}}}
