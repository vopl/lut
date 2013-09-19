#include "lut/async/stable.hpp"
#include "lut/async/impl/threadContainer.hpp"

namespace lut { namespace async { namespace impl
{
    ThreadContainer::ThreadContainer()
    {
    }

    ThreadContainer::~ThreadContainer()
    {
    }

    ThreadReleaseResult ThreadContainer::threadRelease()
    {
        assert(0);
    }

    ThreadReleaseResult ThreadContainer::threadRelease(std::thread::native_handle_type id)
    {
        assert(0);
    }

}}}
