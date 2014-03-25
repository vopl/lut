#ifndef _LUT_ASYNC_HPP_
#define _LUT_ASYNC_HPP_

#include "lut/async/details/taskInstance.hpp"

namespace lut { namespace async
{
    template<class F, class... Args>
    void spawn(F &&f, Args &&...args);

    void yield();
    void utilize();


    ////////////////////////////////////////////////////////////////////////////////
    template<class F, class... Args>
    void spawn(F&& f, Args&&... args)
    {
        details::Task *task = details::TaskInstance<F, Args...>::alloc(std::forward<F>(f), std::forward<Args>(args)...);
        spawn(task);
    }

}}

#endif
