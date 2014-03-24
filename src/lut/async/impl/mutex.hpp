#ifndef _LUT_ASYNC_IMPL_MUTEX_HPP_
#define _LUT_ASYNC_IMPL_MUTEX_HPP_

namespace lut { namespace async { namespace impl
{

    class Mutex
    {
    public:
        Mutex();
        ~Mutex();

    public:
        void acquire();
        bool tryAcquire();

    public:
        void release();

    public:
        bool isAcquired() const;
    };

}}}

#endif
