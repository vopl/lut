#include "lut/async/stable.hpp"
#include "lut/async/scheduler.hpp"
#include "lut/async/impl/coroContainer.hpp"
#include "lut/async/impl/threadContainer.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async
{
    Scheduler::Scheduler()
        : HiddenImpl<impl::CoroContainer>()
        , HiddenImpl<impl::ThreadContainer>()
    {
    }

    Scheduler::~Scheduler()
    {
    }

    namespace
    {
        struct StoppingMeterNull
        {
            void incrementWorkAmount()
            {
            }

            bool completed()
            {
                return false;
            }

            void wait(std::condition_variable &cv, std::unique_lock<std::mutex> &lock)
            {
                cv.wait(lock);
            }
        };
    }

    ThreadUtilizationResult Scheduler::threadUtilize(ThreadState *stateEvt)
    {
        impl::Thread thread(HiddenImpl<impl::ThreadContainer>::impl(), stateEvt);
        StoppingMeterNull stoppingMeter;
        return thread.utilize(stoppingMeter);
    }

    namespace
    {
        struct StoppingMeterByAmount
        {
            const size_t &_amount;
            size_t _counter;

            StoppingMeterByAmount(const size_t &amount)
                : _amount(amount)
                , _counter(0)
            {
            }

            void incrementWorkAmount()
            {
                _counter++;
            }

            bool completed()
            {
                return _counter >= _amount;
            }

            void wait(std::condition_variable &cv, std::unique_lock<std::mutex> &lock)
            {
                cv.wait(lock);
            }
        };

    }

    ThreadUtilizationResult Scheduler::threadUtilize(const size_t &workPiecesAmount, ThreadState *stateEvt)
    {
        impl::Thread thread(HiddenImpl<impl::ThreadContainer>::impl(), stateEvt);
        StoppingMeterByAmount stoppingMeter(workPiecesAmount);
        return thread.utilize(stoppingMeter);
    }

    namespace
    {
        template <class time_point>
        struct StoppingMeterByTime
        {
            const time_point &_time;
            std::cv_status _lastWaitStatus;

            StoppingMeterByTime(const time_point &time)
                : _time(time)
                , _lastWaitStatus(std::cv_status::no_timeout)
            {
            }

            void incrementWorkAmount()
            {
            }

            bool completed()
            {
                return std::cv_status::timeout == _lastWaitStatus || time_point::clock::now() >= _time;
            }

            void wait(std::condition_variable &cv, std::unique_lock<std::mutex> &lock)
            {
                _lastWaitStatus = cv.wait_until(lock, _time);
            }
        };
    }

    ThreadUtilizationResult Scheduler::threadUtilize(const std::chrono::steady_clock::time_point &time, ThreadState *stateEvt)
    {
        impl::Thread thread(HiddenImpl<impl::ThreadContainer>::impl(), stateEvt);
        StoppingMeterByTime<std::chrono::steady_clock::time_point> stoppingMeter(time);

        return thread.utilize(stoppingMeter);
    }

    ThreadUtilizationResult Scheduler::threadUtilize(const std::chrono::high_resolution_clock::time_point &time, ThreadState *stateEvt)
    {
        impl::Thread thread(HiddenImpl<impl::ThreadContainer>::impl(), stateEvt);
        StoppingMeterByTime<std::chrono::high_resolution_clock::time_point> stoppingMeter(time);

        return thread.utilize(stoppingMeter);
    }

    ThreadReleaseResult Scheduler::threadRelease()
    {
        return HiddenImpl<impl::ThreadContainer>::impl().threadRelease();
    }

    ThreadReleaseResult Scheduler::threadRelease(std::thread::native_handle_type id)
    {
        return HiddenImpl<impl::ThreadContainer>::impl().threadRelease(id);
    }

    void Scheduler::spawn(const std::function<void()> &code)
    {
        assert(0);
    }

    void Scheduler::spawn(std::function<void()> &&code)
    {
        assert(0);
    }
}}
