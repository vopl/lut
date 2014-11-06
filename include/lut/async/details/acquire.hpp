#pragma once

#include "lut/async/mutex.hpp"
#include "lut/async/event.hpp"

#include <cstdint>
#include <tuple>

namespace lut { namespace async { namespace impl
{
    class Syncronizer;
    using SyncronizerPtr = Syncronizer *;
}}}

namespace lut { namespace async { namespace details
{

    impl::SyncronizerPtr syncronizer(Mutex &v);
    impl::SyncronizerPtr syncronizer(Event &v);

    std::size_t acquireAny(impl::SyncronizerPtr *syncronizers, std::size_t amount);
    void acquireAll(impl::SyncronizerPtr *syncronizers, std::size_t amount);


    inline void collect(impl::SyncronizerPtr *syncronizers){}
    inline void uncollect(impl::SyncronizerPtr *syncronizers){}

    template <typename First, typename... Acquirable>
    void collect(impl::SyncronizerPtr *syncronizers, First &first, Acquirable&... acquirables)
    {
        *syncronizers = syncronizer(first);
        return collect(syncronizers+1, acquirables...);
    }

    template <typename... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables)
    {
        const std::size_t amount = std::tuple_size<std::tuple<Acquirable...>>::value;
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        std::size_t res = acquireAny(syncronizers, amount);
        return res;
    }

    template <typename... Acquirable>
    void acquireAll(Acquirable&... acquirables)
    {
        const std::size_t amount = std::tuple_size<std::tuple<Acquirable...>>::value;
        impl::SyncronizerPtr syncronizers[amount];
        collect(syncronizers, acquirables...);
        acquireAll(syncronizers, amount);
    }
}}}
