#pragma once

#include "details/taskInstance.hpp"

namespace dci { namespace async
{
    template<class F, class... Args>
    void spawn(F&& f, Args&&... args);

    void yield();

    void executeReadyCoros();

    ////////////////////////////////////////////////////////////////////////////////
    template<class F, class... Args>
    void spawn(F&& f, Args&&... args)
    {
        details::Task *task = details::TaskInstance<F, Args...>::alloc(std::forward<F>(f), std::forward<Args>(args)...);
        spawn(task);
    }

}}
