#ifndef _LUT_ASYNC_DETAILS_TASKINSTANCE_HPP_
#define _LUT_ASYNC_DETAILS_TASKINSTANCE_HPP_

#include "lut/async/details/task.hpp"
#include <cstddef>
#include <boost/pool/pool.hpp>

namespace lut { namespace async { namespace details
{


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size>
    class TaskInstancePool
    {
    public:
        static void *alloc();
        static void free(void *p);

    private:
        static boost::pool<> _pool;
    };


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
    template <size_t size>
    void *TaskInstancePool<size>::alloc()
    {
        return _pool.malloc();
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size>
    void TaskInstancePool<size>::free(void *p)
    {
        return _pool.free(p);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size>
    boost::pool<> TaskInstancePool<size>::_pool(size);


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...> *TaskInstance<F, Args...>::alloc(F &&f, Args &&...args)
    {
        using Pool = TaskInstancePool<sizeof(TaskInstance)>;
        return new(Pool::alloc()) TaskInstance(std::forward<F>(f), std::forward<Args>(args)...);
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

                using Pool = TaskInstancePool<sizeof(TaskInstance)>;
                Pool::free(self);
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
