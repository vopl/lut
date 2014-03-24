#include "lut/async/stable.hpp"
#include "lut/async/conditionVariable.hpp"
#include "lut/async/impl/conditionVariable.hpp"
#include "lut/async/mutex.hpp"
#include "lut/async/semaphore.hpp"

namespace lut { namespace async
{
    ConditionVariable::ConditionVariable()
        : HiddenImpl<impl::ConditionVariable>()
    {
    }

    ConditionVariable::~ConditionVariable()
    {
    }

    void ConditionVariable::acquire(Mutex &mtx)
    {
        return impl().acquire(mtx.impl());
    }

    void ConditionVariable::acquire(Semaphore &sem)
    {
        return impl().acquire(sem.impl());
    }

    bool ConditionVariable::tryAcquire(Mutex &mtx)
    {
        return impl().tryAcquire(mtx.impl());
    }

    bool ConditionVariable::tryAcquire(Semaphore &sem)
    {
        return impl().tryAcquire(sem.impl());
    }

    void ConditionVariable::notifyOne()
    {
        return impl().notifyOne();
    }

    void ConditionVariable::notifyAll()
    {
        return impl().notifyAll();
    }

    bool ConditionVariable::isSignalled() const
    {
        return impl().isSignalled();
    }
}}
