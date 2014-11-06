#pragma once

#include "lut/async/impl/ctx/coro.hpp"
#include <cstdint>

namespace lut { namespace async { namespace impl
{

    class Syncronizer;
    using SyncronizerPtr = Syncronizer *;


    class SyncronizerWaiter
    {
    public:
        SyncronizerWaiter(SyncronizerPtr *syncronizers, std::size_t amount);
        ~SyncronizerWaiter();

    public:
        std::size_t any();
        void all();

    public:
        bool released(std::size_t releasedIdx);

    private:
        SyncronizerPtr *_syncronizers;
        std::size_t _amount;

        ctx::Coro *_coro;

        bool        _waitAll;
        std::size_t _lastReleasedIdx;
    };

}}}
