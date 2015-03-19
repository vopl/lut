#include <details/task.hpp>
#include "../impl/task.hpp"
#include "../impl/scheduler.hpp"

namespace dci { namespace async { namespace details
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
