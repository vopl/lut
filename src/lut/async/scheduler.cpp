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
    ThreadUtilizationResult Scheduler::utilizeThisThread(ThreadState *stateEvt)
    {
        impl::Thread thread(&impl(), stateEvt);
        return thread.utilize();
    }

    ////////////////////////////////////////////////////////////////////////////////
    ThreadReleaseResult Scheduler::releaseThisThread()
    {
        return impl().releaseThread(std::this_thread::get_id());
    }

    ////////////////////////////////////////////////////////////////////////////////
    ThreadReleaseResult Scheduler::releaseThread(std::thread::native_handle_type id)
    {
        return impl().releaseThread(std::thread::id(id));
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::yield()
    {
        return impl().yield();
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Scheduler::spawn(impl::Task &&task)
    {
        return impl().spawn(std::forward<impl::Task>(task));
    }

}}
