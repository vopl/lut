#ifndef _LUT_ASYNC_SEMAPHORE_HPP_
#define _LUT_ASYNC_SEMAPHORE_HPP_

#include "lut/async/hiddenImpl.hpp"
#include <cstdint>

namespace lut { namespace async
{
    namespace impl
    {
        class Semaphore;
    }

    class Semaphore
        : private HiddenImpl<impl::Semaphore>
    {
        Semaphore(const Semaphore &from) = delete;
        Semaphore &operator=(const Semaphore &from) = delete;

    public:
        Semaphore(std::size_t n=1);
        ~Semaphore();

    public:
        void acquire();
        bool tryAcquire();

    public:
        void release();

    public:
        std::size_t counter() const;

    public:
        using HiddenImpl<impl::Semaphore>::pimpl;
        using HiddenImpl<impl::Semaphore>::impl;
    };

}}

#endif
