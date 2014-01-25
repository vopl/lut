#include "lut/async/stable.hpp"
#include "lut/async/details/task.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace details
{
    ////////////////////////////////////////////////////////////////////////////////
    Task::Task(Action action)
        : HiddenImpl<impl::Task>(this, action)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Task::~Task()
    {
    }

}}}
