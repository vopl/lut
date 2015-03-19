#pragma once

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("../sizeProvider.hpp")
#include <dci/himpl/faceLayout.hpp>


namespace dci { namespace async { namespace impl
{
    class Task;
}}}

namespace dci { namespace async { namespace details
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Task
            : public himpl::FaceLayout<impl::Task>
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
        using himpl::FaceLayout<impl::Task>::pimpl;
        using himpl::FaceLayout<impl::Task>::impl;
    };

    void spawn(details::Task *task);
}}}
