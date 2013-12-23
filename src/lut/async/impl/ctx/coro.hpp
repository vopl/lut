#ifndef _LUT_ASYNC_IMPL_CTX_CORO_HPP_
#define _LUT_ASYNC_IMPL_CTX_CORO_HPP_

#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/task.hpp"
#include "lut/async/impl/sm/stack.hpp"

namespace lut { namespace async { namespace impl
{
    class CoroContainer;
}}}

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Coro
        : public Engine
    {
        Coro &operator=(const Coro &) = delete;

        Coro(const lut::async::impl::sm::Stack *stack);
        ~Coro();

    public:
        static Coro *alloc(bool quietFail = false);
        void free();

        void setCode(Task &&code);

    public:
        void switchTo(Engine *to);

    private:
        static void s_contextProc(intptr_t self);
        void contextProc();

    private:
        const lut::async::impl::sm::Stack *_stack;
        Task _task;

    private:
        friend class lut::async::impl::CoroContainer;
        Coro *_nextForContainer;
    };
}}}}

#endif
