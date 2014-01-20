#include "lut/async/stable.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Task::Task()
        : _coro()
        , _preferedThreadId()
        , _priority()
        , _linkToThread()
        , _locksAmount()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Task::~Task()
    {
    }

}}}
