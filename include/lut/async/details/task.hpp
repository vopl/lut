#ifndef _LUT_ASYNC_DETAILS_TASK_HPP_
#define _LUT_ASYNC_DETAILS_TASK_HPP_

#include "lut/async/hiddenImpl.hpp"


namespace lut { namespace async { namespace impl
{
    class Task;
}}}

namespace lut { namespace async { namespace details
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Task
            : public HiddenImpl<impl::Task>
    {
        Task(const Task &) = delete;
        void operator=(const Task &) = delete;

    public:
        enum class ActionKind
        {
            Free,
            Call
        };
        using ActionExecutor = void (*)(Task *task, ActionKind ck);

    protected:
        Task(ActionExecutor actionExecutor);
        ~Task();

    public:
        using HiddenImpl<impl::Task>::pimpl;
        using HiddenImpl<impl::Task>::impl;
    };

}}}

#endif