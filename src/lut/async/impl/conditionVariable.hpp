#ifndef _LUT_ASYNC_IMPL_CONDITIONVARIABLE_HPP_
#define _LUT_ASYNC_IMPL_CONDITIONVARIABLE_HPP_

#include "lut/async/conditionVariable.hpp"
#include "lut/async/impl/syncronizer.hpp"

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

    public:
        template <typename Lock>
        class BindedLock
            : public Syncronizer
        {
        public:
            BindedLock(ConditionVariable &cv, Lock &lock);
            virtual ~BindedLock();

            virtual bool locked() const override;
            virtual void lock() override;

        private:
            ConditionVariable &_cv;
            Lock &_lock;
        };
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
    bool ConditionVariable::BindedLock<Lock>::locked() const
    {
        assert(0);
    }

    template <typename Lock>
    void ConditionVariable::BindedLock<Lock>::lock()
    {
        assert(0);
    }


}}}

#endif
