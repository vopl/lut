#include "lut/stable.hpp"
#include "lut/async/functions.hpp"
#include "lut/async/impl/scheduler.hpp"

namespace lut { namespace async
{
    ////////////////////////////////////////////////////////////////////////////////
    void yield()
    {
        return impl::Scheduler::instance().yield();
    }

    ////////////////////////////////////////////////////////////////////////////////
    void executeReadyCoros()
    {
        return impl::Scheduler::instance().executeReadyCoros();
    }

}}
