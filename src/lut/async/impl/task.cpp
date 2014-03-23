#include "lut/async/stable.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Task::Task(details::Task *face, details::Task::ActionExecutor faceActionExecutor)
        : _face(face)
        , _faceActionExecutor(faceActionExecutor)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Task::~Task()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Task::call()
    {
        _faceActionExecutor(_face, details::Task::ActionKind::Call);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Task::free()
    {
        _faceActionExecutor(_face, details::Task::ActionKind::Free);
    }

}}}
