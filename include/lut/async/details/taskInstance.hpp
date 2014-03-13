#ifndef _LUT_ASYNC_DETAILS_TASKINSTANCE_HPP_
#define _LUT_ASYNC_DETAILS_TASKINSTANCE_HPP_

#include "lut/async/details/task.hpp"
#include "lut/mm/allocator.hpp"
#include <cstddef>
#include <boost/pool/pool.hpp>

namespace lut { namespace async { namespace details
{


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    class TaskInstance
        : public Task
    {
    public:
        static TaskInstance *alloc(F &&f, Args &&...args);

    private:
        static void actionExecutor(Task *task, ActionKind ak);

    private:
        TaskInstance(F &&f, Args &&...args);
        ~TaskInstance();

    private:
        using Call = decltype(std::bind(std::declval<F>(), std::declval<Args>()...));
        Call _call;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...> *TaskInstance<F, Args...>::alloc(F &&f, Args &&...args)
    {
        return new(mm::Allocator::alloc<sizeof(TaskInstance)>()) TaskInstance(std::forward<F>(f), std::forward<Args>(args)...);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    void TaskInstance<F, Args...>::actionExecutor(Task *task, ActionKind ak)
    {
        TaskInstance *self = static_cast<TaskInstance*>(task);

        switch(ak)
        {
        case ActionKind::Call:
            self->_call();
            return;
        case ActionKind::Free:
            {
                self->~TaskInstance();

                mm::Allocator::free<sizeof(TaskInstance)>(self);
                return;
            }
        default:
            assert("unknown ak");
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::TaskInstance(F &&f, Args &&...args)
        : Task(&TaskInstance::actionExecutor)
        , _call(std::forward<F>(f), std::forward<Args>(args)...)
    {
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::~TaskInstance()
    {
    }

}}}

#endif
