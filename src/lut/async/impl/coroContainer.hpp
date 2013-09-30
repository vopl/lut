#ifndef _LUT_ASYNC_IMPL_COROCONTAINER_HPP_
#define _LUT_ASYNC_IMPL_COROCONTAINER_HPP_

#include <atomic>
#include <cstddef>

namespace lut { namespace async { namespace impl
{
    class Coro;

    class CoroContainer
    {
    public:
        CoroContainer();
        ~CoroContainer();

        void enqueue(Coro *coro);
        Coro *dequeue();

        size_t sizeRelaxed() const;
        bool emptyRelaxed() const;

        void gripFrom(CoroContainer &from);

    private:
        void lock();
        void unlock();

    private:
        char pad1[256];
        std::atomic_bool    _lock;
        std::atomic<size_t> _size;
        Coro*               _head;
        Coro*               _tail;
        char pad4[256];
    };
}}}

#endif
