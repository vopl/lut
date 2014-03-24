#ifndef _LUT_ASYNC_CONDITIONVARIABLE_HPP_
#define _LUT_ASYNC_CONDITIONVARIABLE_HPP_

#include "lut/async/hiddenImpl.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class ConditionVariable;
    }

    class Mutex;
    class Semaphore;

    class ConditionVariable
        : private HiddenImpl<impl::ConditionVariable>
    {
        ConditionVariable(const ConditionVariable &from) = delete;
        ConditionVariable &operator=(const ConditionVariable &from) = delete;

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

    public:
        using HiddenImpl<impl::ConditionVariable>::pimpl;
        using HiddenImpl<impl::ConditionVariable>::impl;
    };

}}

#endif
