#ifndef _ASYNC_THREADUTILIZER_HPP_
#define _ASYNC_THREADUTILIZER_HPP_

#include "async/threadState.hpp"

#include <chrono>
#include <thread>

namespace async
{

    enum EThreadUtilizationResult
    {
        etur_limitExhausted,
        etur_releaseRequest,
        etur_notBeenUtilized  //was not be utilized
    };

    enum EThreadReleaseResult
    {
        etrr_ok,
        etrr_notInWork
    };

    class Scheduler;

    namespace impl
    {
        class Scheduler;
        typedef std::shared_ptr<Scheduler> SchedulerPtr;
    }

    class ThreadUtilizer
    {
    public:
        typedef std::chrono::high_resolution_clock _clock;

    public:
        ThreadUtilizer();
        ThreadUtilizer(const impl::SchedulerPtr &implScheduler);

        operator Scheduler();

    public:
        EThreadUtilizationResult te_utilize(ThreadState *stateEvt = NULL);
        EThreadUtilizationResult te_utilize(const _clock::duration &duration, ThreadState *stateEvt = NULL);
        EThreadUtilizationResult te_utilize(const _clock::time_point &time, ThreadState *stateEvt = NULL);
        EThreadUtilizationResult te_utilize(const size_t &workPiecesAmount, ThreadState *stateEvt = NULL);

        EThreadReleaseResult te_release();
        EThreadReleaseResult release(std::thread::native_handle_type id);

    public:
        template<typename rep, typename period>
        EThreadUtilizationResult te_utilize(const std::chrono::duration<rep, period> &duration, ThreadState *stateEvt = NULL);

        template<typename clock, typename duration>
        EThreadUtilizationResult te_utilize(const std::chrono::time_point<clock, duration> &time, ThreadState *stateEvt = NULL);

    private:
        impl::SchedulerPtr _implScheduler;
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename rep, typename period>
    EThreadUtilizationResult ThreadUtilizer::te_utilize(const std::chrono::duration<rep, period> &duration, ThreadState *stateEvt)
    {
        return te_utilize(std::chrono::duration_cast<_clock::duration>(duration), stateEvt);
    }

    template<typename clock, typename duration>
    EThreadUtilizationResult ThreadUtilizer::te_utilize(const std::chrono::time_point<clock, duration> &time, ThreadState *stateEvt)
    {
        return te_utilize(time - clock::now(), stateEvt);
    }

}

#endif
