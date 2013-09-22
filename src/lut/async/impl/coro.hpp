#ifndef _LUT_ASYNC_IMPL_CORO_HPP_
#define _LUT_ASYNC_IMPL_CORO_HPP_

#include "lut/async/impl/context.hpp"
#include "lut/async/impl/intrusiveQueue.hpp"

#include <functional>

namespace lut { namespace async { namespace impl
{

    class Coro
        : public Context
        , public IntrusiveQueue<Coro>::ElementBase
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
        Task _code;
    };
}}}

#endif
