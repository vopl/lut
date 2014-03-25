#ifndef _LUT_ASYNC_IMPL_SEMAPHORE_HPP_
#define _LUT_ASYNC_IMPL_SEMAPHORE_HPP_

#include "lut/async/impl/syncronizer.hpp"
#include <cstdint>

namespace lut { namespace async { namespace impl
{

    class Semaphore
        : public Syncronizer
    {
    public:
        Semaphore(std::size_t n=1);
        virtual ~Semaphore();

    public:
        virtual bool locked() const override;
        virtual void lock() override;
        bool tryLock();
        void unlock();
        std::size_t counter() const;

    private:
        std::size_t _counter;
    };

}}}

#endif
