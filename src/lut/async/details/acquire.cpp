#include "lut/stable.hpp"
#include "lut/async/stable.hpp"
#include "lut/async/details/acquire.hpp"
#include "lut/async/impl/mutex.hpp"
#include "lut/async/impl/event.hpp"
#include "lut/async/impl/syncronizerWaiter.hpp"

#include "lut/mm.hpp"


namespace lut { namespace async { namespace details
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
