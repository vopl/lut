#include "lut/async/stable.hpp"
#include "lut/async/impl/context.hpp"
#include "lut/async/impl/coro.hpp"


namespace lut { namespace async { namespace impl
{

    Context::Context()
    {
        _engine.constructRoot();
    }

    Context::~Context()
    {
        _engine.destructRoot();
    }

    void Context::switchTo(Context *to)
    {
        _engine.switchTo(&to->_engine);
    }

    void Context::switchTo(Coro *to)
    {
        _engine.switchTo(&to->_engine);
    }

}}}
