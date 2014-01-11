#ifndef _LUT_ASYNC_IMPL_TASKINSTANCE_HPP_
#define _LUT_ASYNC_IMPL_TASKINSTANCE_HPP_

#include "lut/async/impl/task.hpp"
#include <functional>
#include <boost/pool/pool.hpp>

namespace lut { namespace async { namespace impl
{


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template <size_t size>
    class TaskInstancePool
    {
    public:
        static void *alloc();
        static void free(void *p);

    private:
        static boost::pool<> _pool;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template<class F, class... Args>
    class TaskInstance
        : public Task
    {
    public:
        static TaskInstance *alloc(F &&f, Args &&...args);

        virtual void free() override;
        virtual void exec() override;

    private:
        TaskInstance(F &&f, Args &&...args);
        ~TaskInstance();

    private:
        using Bind = decltype(std::bind(std::declval<F>(), std::declval<Args>()...));
        Bind _bind;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template <size_t size>
    void *TaskInstancePool<size>::alloc()
    {
        return _pool.malloc();
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template <size_t size>
    void TaskInstancePool<size>::free(void *p)
    {
        return _pool.free(p);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template <size_t size>
    boost::pool<> TaskInstancePool<size>::_pool(size);



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template<class F, class... Args>
    TaskInstance<F, Args...> *TaskInstance<F, Args...>::alloc(F &&f, Args &&...args)
    {
        using Pool = TaskInstancePool<sizeof(TaskInstance)>;
        return new(Pool::alloc()) TaskInstance(std::forward<F>(f), std::forward<Args>(args)...);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template<class F, class... Args>
    void TaskInstance<F, Args...>::free()
    {
        this->~TaskInstance();

        using Pool = TaskInstancePool<sizeof(TaskInstance)>;
        Pool::free(this);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template<class F, class... Args>
    void TaskInstance<F, Args...>::exec()
    {
        _bind();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template<class F, class... Args>
    TaskInstance<F, Args...>::TaskInstance(F &&f, Args &&...args)
        : _bind(std::forward<F>(f), std::forward<Args>(args)...)
    {
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    template<class F, class... Args>
    TaskInstance<F, Args...>::~TaskInstance()
    {

    }

}}}

#endif
