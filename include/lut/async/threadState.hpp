#ifndef _LUT_ASYNC_THREADSTATE_HPP_
#define _LUT_ASYNC_THREADSTATE_HPP_

#include <mutex>
#include <condition_variable>

namespace lut { namespace async
{

    enum class ThreadUtilizationResult
    {
        limitExhausted,
        releaseRequest,
        notBeenUtilized  //was not be utilized
    };

    enum class ThreadReleaseResult
    {
        ok,
        notInWork
    };


    namespace impl
    {
        class Thread;
    }

    enum class ThreadStateValue
    {
        init,
        inited,
        inWork,
        doneWork,
        deinit,
        deinited
    };

    class ThreadState
    {
    public:
        ThreadState();
        ~ThreadState();

        ThreadStateValue get();
        void wait(ThreadStateValue v);
        void waitNot(ThreadStateValue v);

    private:
        friend class impl::Thread;
        void set(ThreadStateValue v);

    private:
        std::mutex              _mtx;
        std::condition_variable _cv;
        ThreadStateValue        _value;
    };
}}

#endif
