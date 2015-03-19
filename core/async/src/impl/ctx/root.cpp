#include "root.hpp"
#include "coro.hpp"


namespace dci { namespace async { namespace impl { namespace ctx
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
