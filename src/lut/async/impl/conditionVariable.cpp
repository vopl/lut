#include "lut/async/stable.hpp"
#include "lut/async/impl/conditionVariable.hpp"


namespace lut { namespace async { namespace impl
{
    ConditionVariable::ConditionVariable()
    {
    }

    ConditionVariable::~ConditionVariable()
    {
    }

    void ConditionVariable::acquire(Mutex &mtx)
    {
        assert(0);
    }

    void ConditionVariable::acquire(Semaphore &sem)
    {
        assert(0);
    }

    bool ConditionVariable::tryAcquire(Mutex &mtx)
    {
        assert(0);
    }

    bool ConditionVariable::tryAcquire(Semaphore &sem)
    {
        assert(0);
    }

    void ConditionVariable::notifyOne()
    {
        assert(0);
    }

    void ConditionVariable::notifyAll()
    {
        assert(0);
    }

    bool ConditionVariable::isSignalled() const
    {
        assert(0);
    }

}}}
