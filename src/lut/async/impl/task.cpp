#include "lut/async/stable.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Task::Task()
        : _nextForWorkerContainer()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Task::~Task()
    {
        assert(!_nextForWorkerContainer);
    }
}}}
