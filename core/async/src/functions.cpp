#include <functions.hpp>
#include "impl/scheduler.hpp"

namespace dci { namespace async
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
