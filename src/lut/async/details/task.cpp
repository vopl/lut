#include "lut/async/stable.hpp"
#include "lut/async/details/task.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace details
{
    ////////////////////////////////////////////////////////////////////////////////
    Task::Task(ActionExecutor actionExecutor)
        : HiddenImpl<impl::Task>(this, actionExecutor)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Task::~Task()
    {
    }

}}}
