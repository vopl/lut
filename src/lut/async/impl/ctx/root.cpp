#include "lut/async/stable.hpp"
#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/ctx/coro.hpp"


namespace lut { namespace async { namespace impl { namespace ctx
{

    Root::Root()
    {
        _engine.constructRoot();
    }

    Root::~Root()
    {
        _engine.destructRoot();
    }

    void Root::switchTo(Root *to)
    {
        _engine.switchTo(&to->_engine);
    }

    void Root::switchTo(Coro *to)
    {
        _engine.switchTo(&to->_engine);
    }

}}}}
