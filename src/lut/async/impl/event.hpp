#pragma once

#include "lut/async/impl/syncronizer.hpp"

namespace lut { namespace async { namespace impl
{

    class Event
        : public Syncronizer
    {
    public:
        Event(bool autoReset);
        virtual ~Event();

    public:
        virtual bool locked() const override;
        virtual void lock() override;
        bool tryLock();
        bool signalled() const;

        void set();
        void reset();

    private:
        bool _autoReset;
        bool _signalled;

    };

}}}
