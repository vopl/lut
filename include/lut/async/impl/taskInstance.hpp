#ifndef _LUT_ASYNC_IMPL_TASKINSTANCE_HPP_
#define _LUT_ASYNC_IMPL_TASKINSTANCE_HPP_

#include "lut/async/impl/task.hpp"
#include <cstddef>
#include <boost/pool/pool.hpp>

namespace lut { namespace async { namespace impl
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
    template<size_t LocksAmount, class F, class... Args>
    class TaskInstance
        : public Task
    {
    public:
        static TaskInstance *alloc(F &&f, Args &&...args);

        void free() override;
        void call() override;

    private:
        TaskInstance(F &&f, Args &&...args);
        ~TaskInstance();

    private:
        using Call = decltype(std::bind(std::declval<F>(), std::declval<Args>()...));
        Call &getCall();

        static const size_t sizeofClass = sizeof(Task) - sizeof(_locks) + sizeof(LockPtr)*LocksAmount + sizeof(Call);
        using Pool = TaskInstancePool<sizeofClass>;
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
    template<size_t LocksAmount, class F, class... Args>
    TaskInstance<LocksAmount, F, Args...> *TaskInstance<LocksAmount, F, Args...>::alloc(F &&f, Args &&...args)
    {
        static_assert(sizeof(Task) == sizeof(TaskInstance), "task and instance sizes must be equal");
        return new(Pool::alloc()) TaskInstance(std::forward<F>(f), std::forward<Args>(args)...);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<size_t LocksAmount, class F, class... Args>
    void TaskInstance<LocksAmount, F, Args...>::free()
    {
        this->~TaskInstance();

        Pool::free(this);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<size_t LocksAmount, class F, class... Args>
    void TaskInstance<LocksAmount, F, Args...>::call()
    {
        getCall()();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<size_t LocksAmount, class F, class... Args>
    TaskInstance<LocksAmount, F, Args...>::TaskInstance(F &&f, Args &&...args)
        : Task()
    {
        new(&getCall()) Call(std::forward<F>(f), std::forward<Args>(args)...);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<size_t LocksAmount, class F, class... Args>
    TaskInstance<LocksAmount, F, Args...>::~TaskInstance()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<size_t LocksAmount, class F, class... Args>
    typename TaskInstance<LocksAmount, F, Args...>::Call &TaskInstance<LocksAmount, F, Args...>::getCall()
    {
        using LockPtr = LockPtr;
        union
        {
            LockPtr *plock;
            char *pchar;
            Call *pcall;
        } addr;
        addr.plock = _locks;
        addr.pchar += sizeof(LockPtr)*LocksAmount;
        return *addr.pcall;
    }

}}}

#endif
