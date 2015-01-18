#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/async/sizeProvider.hpp"
#endif
#include "lut/hiddenImpl/single.hpp"


namespace lut { namespace async { namespace impl
{
    class Task;
}}}

namespace lut { namespace async { namespace details
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Task
            : public hiddenImpl::Single<impl::Task>
    {
        Task(const Task &) = delete;
        void operator=(const Task &) = delete;

    public:
        using CallExecutor = void (*)(Task *task);
        using FreeExecutor = void (*)(Task *task);

    protected:
        Task(CallExecutor callExecutor, FreeExecutor freeExecutor);
        ~Task();

    public:
        using Single<impl::Task>::pimpl;
        using Single<impl::Task>::impl;
    };

    void spawn(details::Task *task);
}}}
