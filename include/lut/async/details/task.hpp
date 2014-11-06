#pragma once

#if !GENERATE_SIZEOFIMPL
#   include "lut/async/sizeofImpl.hpp"
#endif
#include "lut/hiddenImpl.hpp"


namespace lut { namespace async { namespace impl
{
    class Task;
}}}

namespace lut { namespace async { namespace details
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Task
            : public HiddenImpl<impl::Task>
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
        using HiddenImpl<impl::Task>::pimpl;
        using HiddenImpl<impl::Task>::impl;
    };

    void spawn(details::Task *task);
}}}
