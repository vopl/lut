#pragma once

#include "syncronizer.hpp"
#include <cstdint>

namespace dci { namespace async { namespace impl
{

    class Mutex
        : public Syncronizer
    {
    public:
        Mutex();
        virtual ~Mutex();

    public:
        virtual bool locked() const override;
        virtual void lock() override;
        bool tryLock();
        void unlock();

    private:
        bool _locked;
    };

}}}
