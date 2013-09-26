#ifndef _LUT_ASYNC_SCHEDULER_HPP_
#define _LUT_ASYNC_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/task.hpp"
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
        ThreadUtilizationResult threadUtilize(ThreadState *stateEvt = NULL);
        ThreadReleaseResult threadRelease();
        ThreadReleaseResult threadRelease(std::thread::native_handle_type id);

    public://code
        template<class F, class... Args>
        void spawn(F &&f, Args &&...args);

        void yield();

    private:
        void spawn(impl::Task &&task);
    };


    ////////////////////////////////////////////////////////////////////////////////
    template<class F, class... Args>
    void Scheduler::spawn(F&& f, Args&&... args)
    {
        spawn(impl::Task(std::forward<F>(f), std::forward<Args>(args)...));
    }

}}

#endif
