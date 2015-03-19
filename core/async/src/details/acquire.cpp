#include <details/acquire.hpp>
#include "../impl/mutex.hpp"
#include "../impl/event.hpp"
#include "../impl/syncronizerWaiter.hpp"

#include <dci/mm/functions.hpp>


namespace dci { namespace async { namespace details
{

    impl::SyncronizerPtr syncronizer(Mutex &v)
    {
        return v.pimpl();
    }

    impl::SyncronizerPtr syncronizer(Event &v)
    {
        return v.pimpl();
    }

    std::size_t acquireAny(impl::SyncronizerPtr *syncronizers, std::size_t amount)
    {
        impl::SyncronizerWaiter syncronizerWaiter(syncronizers, amount);
        return syncronizerWaiter.any();
    }

    void acquireAll(impl::SyncronizerPtr *syncronizers, std::size_t amount)
    {
        impl::SyncronizerWaiter syncronizerWaiter(syncronizers, amount);
        return syncronizerWaiter.all();
    }

}}}
