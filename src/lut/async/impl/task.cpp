#include "lut/async/stable.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Task::Task(details::Task *face, details::Task::CallExecutor faceCallExecutor, details::Task::FreeExecutor faceFreeExecutor)
        : _face(face)
        , _faceCallExecutor(faceCallExecutor)
        , _faceFreeExecutor(faceFreeExecutor)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Task::~Task()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Task::call()
    {
        _faceCallExecutor(_face);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Task::free()
    {
        _faceFreeExecutor(_face);
    }

}}}
