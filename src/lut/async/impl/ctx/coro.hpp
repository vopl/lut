#ifndef _LUT_ASYNC_IMPL_CORO_CTX_HPP_
#define _LUT_ASYNC_IMPL_CORO_CTX_HPP_

#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{
    class CoroContainer;
}}}

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Coro
    {
        Coro &operator=(const Coro &) = delete;

    public:
        static Coro *make(size_t stackSize = 1024*32);
        Coro();
        void operator delete(void *p);
        ~Coro();

        void setCode(Task &&code);

        void switchTo(Root *to);
        void switchTo(Coro *to);

    private:
        static void s_contextProc(intptr_t self);
        void contextProc();

    private:
        Task _task;

    private:
        friend class lut::async::impl::CoroContainer;
        Coro *_nextForContainer;

    private://last
        friend class Root;
        Engine _engine;
    };
}}}}

#endif
