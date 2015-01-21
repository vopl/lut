#include "lut/stable.hpp"
#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/ctx/coro.hpp"


namespace lut { namespace async { namespace impl { namespace ctx
{

    Root::Root()
    {
        constructRoot();
    }

    Root::~Root()
    {
        destructRoot();
    }

}}}}
