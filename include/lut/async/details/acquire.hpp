#ifndef _LUT_ASYNC_DETAILS_ACQUIRE_HPP_
#define _LUT_ASYNC_DETAILS_ACQUIRE_HPP_

#include "lut/async/mutex.hpp"
#include "lut/async/recursiveMutex.hpp"
#include "lut/async/semaphore.hpp"
#include "lut/async/event.hpp"
#include "lut/async/conditionVariable.hpp"

#include <cstdint>
#include <tuple>

namespace lut { namespace async { namespace impl
{
    class Syncronizer;
    using SyncronizerPtr = Syncronizer *;
}}}

namespace lut { namespace async { namespace details
{

    impl::SyncronizerPtr syncronizerCreate(Mutex &v);
    impl::SyncronizerPtr syncronizerCreate(RecursiveMutex &v);
    impl::SyncronizerPtr syncronizerCreate(Semaphore &v);
    impl::SyncronizerPtr syncronizerCreate(Event &v);
    impl::SyncronizerPtr syncronizerCreate(ConditionVariable::BindedLock<Mutex> &v);
    impl::SyncronizerPtr syncronizerCreate(ConditionVariable::BindedLock<RecursiveMutex> &v);
    impl::SyncronizerPtr syncronizerCreate(ConditionVariable::BindedLock<Semaphore> &v);

    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, Mutex &v);
    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, RecursiveMutex &v);
    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, Semaphore &v);
    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, Event &v);
    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, ConditionVariable::BindedLock<Mutex> &v);
    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, ConditionVariable::BindedLock<RecursiveMutex> &v);
    void syncronizerDestroy(impl::SyncronizerPtr syncronizer, ConditionVariable::BindedLock<Semaphore> &v);

    std::size_t acquireAny(impl::SyncronizerPtr *syncronizers, std::size_t amount);
    void acquireAll(impl::SyncronizerPtr *syncronizers, std::size_t amount);


    inline void collect(impl::SyncronizerPtr *syncronizers){}
    inline void uncollect(impl::SyncronizerPtr *syncronizers){}

    template <typename First, typename... Acquirable>
    void collect(impl::SyncronizerPtr *syncronizers, First &first, Acquirable&... acquirables)
    {
        *syncronizers = syncronizerCreate(first);
        return collect(syncronizers+1, acquirables...);
    }

    template <typename First, typename... Acquirable>
    void uncollect(impl::SyncronizerPtr *syncronizers, First &first, Acquirable&... acquirables)
    {
        syncronizerDestroy(*syncronizers, first);
        return uncollect(syncronizers+1, acquirables...);
    }

    template <typename... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables)
    {
        const std::size_t amount = std::tuple_size<std::tuple<Acquirable...>>::value;
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        std::size_t res = acquireAny(syncronizers, amount);
        uncollect(syncronizers, acquirables...);
        return res;
    }

    template <typename... Acquirable>
    void acquireAll(Acquirable&... acquirables)
    {
        const std::size_t amount = std::tuple_size<std::tuple<Acquirable...>>::value;
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        acquireAll(syncronizers, amount);
        uncollect(syncronizers, acquirables...);
    }
}}}

#endif
