#ifndef _LUT_ASYNC_SCHEDULER_HPP_
#define _LUT_ASYNC_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/taskInstance.hpp"
#include "lut/async/hiddenImpl.hpp"

#include <thread>

namespace lut { namespace async
{
    namespace impl
    {
        class Scheduler;
    }

    class Scheduler
        : private HiddenImpl<impl::Scheduler>
    {
        Scheduler(const Scheduler &from) = delete;
        Scheduler &operator=(const Scheduler &from) = delete;

    public:
        Scheduler();
        ~Scheduler();

    public://threads
        ThreadUtilizationResult utilizeThisThread(ThreadState *stateEvt = NULL);
        ThreadReleaseResult releaseThisThread();
        ThreadReleaseResult releaseThread(std::thread::native_handle_type id);

    public://code
        template<class F, class... Args>
        void spawn(F &&f, Args &&...args);

        void yield();

    private:
        void spawn(impl::Task *task);
    };


    ////////////////////////////////////////////////////////////////////////////////
    template<class F, class... Args>
    void Scheduler::spawn(F&& f, Args&&... args)
    {
        impl::Task *task = impl::TaskInstance<F, Args...>::alloc(std::forward<F>(f), std::forward<Args>(args)...);
        spawn(task);
    }

}}

#endif
