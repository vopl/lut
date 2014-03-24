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
        template <typename Lock>
        class BindedLock
        {
        public:
            BindedLock(ConditionVariable &cv, Lock &lock);
            ~BindedLock();
            void acquire();

        public:
            ConditionVariable &conditionVariable() const;
            Lock &lock() const;

        private:
            ConditionVariable &_cv;
            Lock &_lock;
        };

        template <typename Lock>
        BindedLock<Lock> bind(Lock &lock);

    public:
        using HiddenImpl<impl::ConditionVariable>::pimpl;
        using HiddenImpl<impl::ConditionVariable>::impl;
    };


    template <typename Lock>
    ConditionVariable::BindedLock<Lock>::BindedLock(ConditionVariable &cv, Lock &lock)
        : _cv(cv)
        , _lock(lock)
    {
    }

    template <typename Lock>
    ConditionVariable::BindedLock<Lock>::~BindedLock()
    {

    }

    template <typename Lock>
    void ConditionVariable::BindedLock<Lock>::acquire()
    {
        return _cv.acquire(_lock);
    }

    template <typename Lock>
    ConditionVariable &ConditionVariable::BindedLock<Lock>::conditionVariable() const
    {
        return _cv;
    }

    template <typename Lock>
    Lock &ConditionVariable::BindedLock<Lock>::lock() const
    {
        return _lock;
    }

    template <typename Lock>
    ConditionVariable::BindedLock<Lock> ConditionVariable::bind(Lock &lock)
    {
        return BindedLock<Lock>(*this, lock);
    }

}}

#endif
