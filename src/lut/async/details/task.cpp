#include "lut/stable.hpp"
#include "lut/async/details/task.hpp"
#include "lut/async/impl/task.hpp"
#include "lut/async/impl/scheduler.hpp"

namespace lut { namespace async { namespace details
{
    ////////////////////////////////////////////////////////////////////////////////
    Task::Task(CallExecutor callExecutor, FreeExecutor freeExecutor)
        : himpl::FaceLayout<impl::Task>(this, callExecutor, freeExecutor)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Task::~Task()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    void spawn(details::Task *task)
    {
        return impl::Scheduler::instance().spawn(task->pimpl());
    }

}}}
