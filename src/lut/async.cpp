#include "lut/async/stable.hpp"
#include "lut/async.hpp"
#include "lut/async/impl/scheduler.hpp"

namespace lut { namespace async
{
    ////////////////////////////////////////////////////////////////////////////////
    void yield()
    {
        return impl::Scheduler::instance().yield();
    }

    ////////////////////////////////////////////////////////////////////////////////
    void run()
    {
        return impl::Scheduler::instance().run();
    }

    ////////////////////////////////////////////////////////////////////////////////
    void stop()
    {
        return impl::Scheduler::instance().stop();
    }

}}
