#include "lut/async/stable.hpp"
#include "lut/async/details/acquire.hpp"
#include "lut/async/impl/mutex.hpp"
#include "lut/async/impl/recursiveMutex.hpp"
#include "lut/async/impl/semaphore.hpp"
#include "lut/async/impl/event.hpp"
#include "lut/async/impl/conditionVariable.hpp"

#include "lut/mm/allocator.hpp"


namespace lut { namespace async { namespace details
{

    impl::SyncronizerPtr syncronizerCreate(Mutex &v)
    {
        return v.pimpl();
    }

    impl::SyncronizerPtr syncronizerCreate(RecursiveMutex &v)
    {
        return v.pimpl();
    }

    impl::SyncronizerPtr syncronizerCreate(Semaphore &v)
    {
        return v.pimpl();
    }

    impl::SyncronizerPtr syncronizerCreate(Event &v)
    {
        return v.pimpl();
    }

    impl::SyncronizerPtr syncronizerCreate(ConditionVariable::BindedLock<Mutex> &v)
    {
        using Target = impl::ConditionVariable::BindedLock<impl::Mutex>;
        return new(mm::Allocator::alloc<sizeof(Target)>()) Target(v.conditionVariable().impl(), v.lock().impl());
    }

    impl::SyncronizerPtr syncronizerCreate(ConditionVariable::BindedLock<RecursiveMutex> &v)
    {
        using Target = impl::ConditionVariable::BindedLock<impl::RecursiveMutex>;
        return new(mm::Allocator::alloc<sizeof(Target)>()) Target(v.conditionVariable().impl(), v.lock().impl());
    }

    impl::SyncronizerPtr syncronizerCreate(ConditionVariable::BindedLock<Semaphore> &v)
    {
        using Target = impl::ConditionVariable::BindedLock<impl::Semaphore>;
        return new(mm::Allocator::alloc<sizeof(Target)>()) Target(v.conditionVariable().impl(), v.lock().impl());
    }

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, Mutex &v)
    {
        //empty ok;
    }

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, RecursiveMutex &v)
    {
        //empty ok;
    }

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, Semaphore &v)
    {
        //empty ok;
    }

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, Event &v)
    {
        //empty ok;
    }

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, ConditionVariable::BindedLock<Mutex> &v)
    {
        using Target = impl::ConditionVariable::BindedLock<impl::Mutex>;
        Target *target = static_cast<Target *>(syncronizer);
        target->~Target();
        return mm::Allocator::free<sizeof(Target)>(target);
    }

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, ConditionVariable::BindedLock<RecursiveMutex> &v)
    {
        using Target = impl::ConditionVariable::BindedLock<impl::RecursiveMutex>;
        Target *target = static_cast<Target *>(syncronizer);
        target->~Target();
        return mm::Allocator::free<sizeof(Target)>(target);
    }

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, ConditionVariable::BindedLock<Semaphore> &v)
    {
        using Target = impl::ConditionVariable::BindedLock<impl::Semaphore>;
        Target *target = static_cast<Target *>(syncronizer);
        target->~Target();
        return mm::Allocator::free<sizeof(Target)>(target);
    }

    std::size_t acquireAny(impl::SyncronizerPtr *syncronizers, std::size_t amount)
    {
        assert(0);
    }

    void acquireAll(impl::SyncronizerPtr *syncronizers, std::size_t amount)
    {
        assert(0);

//        std::size_t acquired = 0;

//        for(std::size_t idx(0); idx<amount; ++idx)
//        {
//            if(syncronizers[idx]->registerWaiter(currentCoro))
//            {
//                acquired++;
//            }
//        }

//        if(acquired < amount)
//        {
//            acquired = 0;

//            for(std::size_t idx(0); acquired<amount; ++idx)
//            {
//                if(idx == amount)
//                {
//                    idx = 0;
//                }

//                if(syncronizers[idx]->canAcquire())
//                {
//                    acquired++;
//                }
//                else
//                {
//                    deactivate();
//                    acquired = 0;
//                }
//            }
//        }

//        assert(amount = acquired);

//        for(std::size_t idx(0); idx<amount; ++idx)
//        {
//            syncronizers[idx]->unregisterWaiterAndCommitAcquire(currentCoro);
//        }
    }

}}}
