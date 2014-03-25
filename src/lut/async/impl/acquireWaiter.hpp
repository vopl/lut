#ifndef _LUT_ASYNC_IMPL_ACQUIREWAITER_HPP_
#define _LUT_ASYNC_IMPL_ACQUIREWAITER_HPP_

#include <cstdint>

namespace lut { namespace async { namespace impl
{

    class Syncronizer;
    using SyncronizerPtr = Syncronizer *;


    class AcquireWaiter
    {
    public:
        AcquireWaiter(SyncronizerPtr *syncronizers, std::size_t amount);
        ~AcquireWaiter();

    public:
        std::size_t any();
        void all();

    private:
        SyncronizerPtr *_syncronizers;
        std::size_t _amount;
        std::size_t _lastNotifiedIdx;
    };

}}}

#endif
