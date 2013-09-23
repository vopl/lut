#include "lut/async/stable.hpp"
#include "lut/async/scheduler.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async
{
    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::Scheduler()
        : HiddenImpl<impl::Scheduler>()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scheduler::~Scheduler()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    ThreadUtilizationResult Scheduler::threadUtilize(ThreadState *stateEvt)
    {
        impl::Thread thread(&impl(), stateEvt);
        return thread.utilize();
    }

    ////////////////////////////////////////////////////////////////////////////////
    ThreadReleaseResult Scheduler::threadRelease()
    {
        return impl().threadRelease(std::this_thread::get_id());
    }

    ////////////////////////////////////////////////////////////////////////////////
    ThreadReleaseResult Scheduler::threadRelease(std::thread::native_handle_type id)
    {
        return impl().threadRelease(std::thread::id(id));
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::spawn(const std::function<void()> &code)
    {
        return impl().spawn(code);
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::spawn(std::function<void()> &&code)
    {
        return impl().spawn(std::forward<std::function<void()>>(code));
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::yield()
    {
        return impl().yield();
    }

}}
