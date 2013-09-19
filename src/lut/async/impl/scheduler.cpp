#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"

namespace lut { namespace async { namespace impl
{
    Scheduler::Scheduler()
    {
    }

    Scheduler::~Scheduler()
    {
    }

    ThreadReleaseResult Scheduler::threadRelease()
    {
        assert(0);
    }

    ThreadReleaseResult Scheduler::threadRelease(std::thread::native_handle_type id)
    {
        assert(0);
    }

}}}
