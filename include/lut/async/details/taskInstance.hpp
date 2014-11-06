#pragma once

#include "lut/async/details/task.hpp"
#include "lut/mm.hpp"

#include <cstddef>
#include <functional>

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
        static void callExecutor(Task *task);
        static void freeExecutor(Task *task);

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
        return new(mm::alloc<sizeof(TaskInstance)>()) TaskInstance(std::forward<F>(f), std::forward<Args>(args)...);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    void TaskInstance<F, Args...>::callExecutor(Task *task)
    {
        TaskInstance *self = static_cast<TaskInstance*>(task);

        self->_call();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    void TaskInstance<F, Args...>::freeExecutor(Task *task)
    {
        TaskInstance *self = static_cast<TaskInstance*>(task);

        self->~TaskInstance();

        mm::free<sizeof(TaskInstance)>(self);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::TaskInstance(F &&f, Args &&...args)
        : Task(&TaskInstance::callExecutor, &TaskInstance::freeExecutor)
        , _call(std::forward<F>(f), std::forward<Args>(args)...)
    {
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::~TaskInstance()
    {
    }

}}}
