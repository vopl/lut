#include "lut/async/stable.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Task::Task(details::Task *face, details::Task::Action faceAction)
        : _face(face)
        , _faceAction(faceAction)
        , _coro()
        , _priority()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Task::~Task()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Task::call()
    {
        _faceAction(_face, details::Task::ActionKind::Call);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Task::free()
    {
        _faceAction(_face, details::Task::ActionKind::Free);
    }

}}}
