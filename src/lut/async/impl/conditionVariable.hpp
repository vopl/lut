#ifndef _LUT_ASYNC_IMPL_CONDITIONVARIABLE_HPP_
#define _LUT_ASYNC_IMPL_CONDITIONVARIABLE_HPP_

namespace lut { namespace async { namespace impl
{

    class Mutex;
    class Semaphore;

    class ConditionVariable
    {
    public:
        ConditionVariable();
        ~ConditionVariable();

    public:
        void acquire(Mutex &mtx);
        void acquire(Semaphore &sem);

        bool tryAcquire(Mutex &mtx);
        bool tryAcquire(Semaphore &sem);

    public:
        void notifyOne();
        void notifyAll();

    public:
        bool isSignalled() const;

    };

}}}

#endif
