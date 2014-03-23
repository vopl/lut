#ifndef _LUT_ASYNC_IMPL_TASK_HPP_
#define _LUT_ASYNC_IMPL_TASK_HPP_

#include "lut/async/details/task.hpp"
#include <thread>

namespace lut { namespace async { namespace impl
{
    template <typename T>
    class EffortContainer;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Task
    {
    public:
        Task(details::Task *face, details::Task::ActionExecutor faceActionExecutor);
        ~Task();

    public:
        void call();
        void free();

    private:
        details::Task *_face;
        details::Task::ActionExecutor _faceActionExecutor;

        friend class ::lut::async::impl::EffortContainer<Task>;
        Task *  _nextInList;
    };

}}}

#endif
