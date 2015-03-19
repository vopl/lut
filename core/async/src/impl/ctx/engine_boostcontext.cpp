#include "dci/stable.hpp"
#include "dci/async/impl/ctx/engine_boostcontext.hpp"
#include "dci/mm/config.hpp"

#include <cstddef>

namespace dci { namespace async { namespace impl { namespace ctx
{
    ////////////////////////////////////////////////////////////////////////////////
    Engine::Engine()
        : _ctx()
        , _ctxIsRoot()
        , _coroArg()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructRoot()
    {
        assert(!_ctx);
        assert(!_ctxIsRoot);
        assert(!_coroArg);

        _ctx = new boost::context::fcontext_t;
        _ctxIsRoot = true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructRoot()
    {
        assert(_ctx);
        assert(_ctxIsRoot);
        assert(!_coroArg);

        delete _ctx;
        _ctx = nullptr;
        _ctxIsRoot = false;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::constructCoro(char *stackPtr, std::size_t stackSize, void(*f)(std::intptr_t), std::uintptr_t arg)
    {
        assert(!_ctx);
        assert(!_ctxIsRoot);
        assert(!_coroArg);

        _coroArg = arg;

        if(mm::Config::_stackGrowsDown)
        {
            _ctx = boost::context::make_fcontext(
                      stackPtr + stackSize,
                      stackSize,
                      f);
        }
        else
        {
            _ctx = boost::context::make_fcontext(
                      stackPtr,
                      stackSize,
                      f);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::destructCoro()
    {
        assert(_ctx);
        assert(!_ctxIsRoot);

        _ctx = nullptr;
        _coroArg = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Engine::switchTo(Engine *to)
    {
        boost::context::jump_fcontext(_ctx, to->_ctx, to->_coroArg, false);
    }
}}}}
