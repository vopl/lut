#ifndef _LUT_ASYNC_IMPL_CORO_HPP_
#define _LUT_ASYNC_IMPL_CORO_HPP_

#include "lut/async/impl/context.hpp"
#include "lut/async/impl/queue.hpp"

#include <functional>

namespace lut { namespace async { namespace impl
{

    class Coro
        : public Context
        , public Queue<Coro>::ElementBase
    {
    public:
        using Task = std::function<void()>;

    public:
        Coro();
        ~Coro();

        void setCode(const Task &code);
        void setCode(Task &&code);

    private:
        void contextProc() override;

    private:
        Coro *_readyList_next;
        Coro *_emptyList_next;
    };
}}}

#endif
