#pragma once

#include "lut/async/sizeProvider.hpp"
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace async
{
    namespace impl
    {
        class Mutex;
    }

    class Mutex
        : public himpl::FaceLayout<impl::Mutex>
    {
        Mutex(const Mutex &from) = delete;
        Mutex &operator=(const Mutex &from) = delete;

    public:
        Mutex();
        ~Mutex();

    public:
        void lock();
        bool tryLock();

    public:
        void unlock();

    public:
        bool locked() const;

    public:
        using himpl::FaceLayout<impl::Mutex>::pimpl;
        using himpl::FaceLayout<impl::Mutex>::impl;
    };

}}
