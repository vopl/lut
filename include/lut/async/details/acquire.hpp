#pragma once

#include "lut/async/mutex.hpp"
#include "lut/async/event.hpp"
#include "lut/async/future.hpp"

#include <cstdint>
#include <tuple>

namespace lut { namespace async { namespace impl
{
    class Syncronizer;
    using SyncronizerPtr = Syncronizer *;
}}}

namespace lut { namespace async { namespace details
{

    ////////////////////////////////////////////////////////////////////////////////////
    impl::SyncronizerPtr syncronizer(Mutex &v);
    impl::SyncronizerPtr syncronizer(Event &v);

    template <typename... TT>
    impl::SyncronizerPtr syncronizer(Future<TT...> &v)
    {
        return syncronizer(FutureStateAccessos::exec(v)._readyEvent);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    std::size_t acquireAny(impl::SyncronizerPtr *syncronizers, std::size_t amount);
    void acquireAll(impl::SyncronizerPtr *syncronizers, std::size_t amount);

    ////////////////////////////////////////////////////////////////////////////////////
    inline std::size_t acquerablesAmount()
    {
        return 0;
    }

    template <typename First, typename... Acquirable>
    std::size_t acquerablesAmount(First &first, Acquirable&... acquirables)
    {
        return 1 + acquerablesAmount(acquirables...);
    }

    template <template <typename...> class Container, typename First, typename... Acquirable, typename... ContainerExtraArgs>
    std::size_t acquerablesAmount(Container<First, ContainerExtraArgs...> &first, Acquirable&... acquirables)
    {
        return first.size() + acquerablesAmount(acquirables...);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    inline void collect(impl::SyncronizerPtr *syncronizers){}

    template <typename First, typename... Acquirable>
    void collect(impl::SyncronizerPtr *syncronizers, First &first, Acquirable&... acquirables)
    {
        *syncronizers = syncronizer(first);
        return collect(syncronizers+1, acquirables...);
    }

    template <template <typename...> class Container, typename First, typename... Acquirable, typename... ContainerExtraArgs>
    void collect(impl::SyncronizerPtr *syncronizers, Container<First, ContainerExtraArgs...> &first, Acquirable&... acquirables)
    {
        using Cnt = Container<First, ContainerExtraArgs...>;

        typename Cnt::iterator iter = std::begin(first);
        typename Cnt::iterator end = std::end(first);

        for(; iter!=end; ++iter)
        {
            *syncronizers = syncronizer(*iter);
            syncronizers++;
        }

        return collect(syncronizers, acquirables...);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    template <typename... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables)
    {
        const std::size_t amount = acquerablesAmount(acquirables...);
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        std::size_t res = acquireAny(syncronizers, amount);
        return res;
    }

    template <typename... Acquirable>
    void acquireAll(Acquirable&... acquirables)
    {
        const std::size_t amount = acquerablesAmount(acquirables...);
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        acquireAll(syncronizers, amount);
    }
}}}
