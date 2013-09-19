#ifndef _LUT_ASYNC_SCHEDULER_HPP_
#define _LUT_ASYNC_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/hiddenImpl.hpp"

#include <thread>
#include <functional>

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
        ThreadUtilizationResult threadUtilize(const size_t &workPiecesAmount, ThreadState *stateEvt = NULL);
        ThreadUtilizationResult threadUtilize(const std::chrono::steady_clock::time_point &time, ThreadState *stateEvt = NULL);
        ThreadUtilizationResult threadUtilize(const std::chrono::high_resolution_clock::time_point &time, ThreadState *stateEvt = NULL);

        template<typename rep, typename period>
        ThreadUtilizationResult threadUtilize(const std::chrono::duration<rep, period> &duration, ThreadState *stateEvt = NULL);

        template<typename clock, typename duration>
        ThreadUtilizationResult threadUtilize(const std::chrono::time_point<clock, duration> &time, ThreadState *stateEvt = NULL);

        template<typename duration>
        ThreadUtilizationResult threadUtilize(const std::chrono::time_point<std::chrono::steady_clock, duration> &time, ThreadState *stateEvt = NULL);

        ThreadReleaseResult threadRelease();
        ThreadReleaseResult threadRelease(std::thread::native_handle_type id);

    public://code
        void spawn(const std::function<void()> &code);
        void spawn(std::function<void()> &&code);
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename rep, typename period>
    ThreadUtilizationResult Scheduler::threadUtilize(const std::chrono::duration<rep, period> &duration, ThreadState *stateEvt)
    {
        return threadUtilize(std::chrono::steady_clock::now() + duration, stateEvt);
    }

    template<typename clock, typename duration>
    ThreadUtilizationResult Scheduler::threadUtilize(const std::chrono::time_point<clock, duration> &time, ThreadState *stateEvt)
    {
        return threadUtilize(std::chrono::time_point_cast<std::chrono::high_resolution_clock>(time), stateEvt);
    }

    template<typename duration>
    ThreadUtilizationResult Scheduler::threadUtilize(const std::chrono::time_point<std::chrono::steady_clock, duration> &time, ThreadState *stateEvt)
    {
        return threadUtilize(std::chrono::time_point_cast<std::chrono::steady_clock>(time), stateEvt);
    }

}}

#endif
