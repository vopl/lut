#pragma once

#include "details/task.hpp"
#include <thread>

namespace dci { namespace async { namespace impl
{
    template <typename T>
    class EffortContainer;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Task
    {
    public:
        Task(details::Task *face, details::Task::CallExecutor faceCallExecutor, details::Task::FreeExecutor faceFreeExecutor);
        ~Task();

    public:
        void call();
        void free();

    private:
        details::Task *_face;
        details::Task::CallExecutor _faceCallExecutor;
        details::Task::FreeExecutor _faceFreeExecutor;

        friend class ::dci::async::impl::EffortContainer<Task>;
        Task *  _nextInList;
    };

}}}
