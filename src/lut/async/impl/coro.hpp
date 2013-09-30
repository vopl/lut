#ifndef _LUT_ASYNC_IMPL_CORO_HPP_
#define _LUT_ASYNC_IMPL_CORO_HPP_

#include "lut/async/impl/context.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{

    class CoroContainer;

    class Coro
    {
        Coro &operator=(const Coro &) = delete;

    public:
        static Coro *make(size_t stackSize = 1024*32);
        Coro();
        void operator delete(void *p);
        ~Coro();

        void setCode(Task &&code);

        void switchTo(Context *to);
        void switchTo(Coro *to);

    private:
        static void s_contextProc(intptr_t self);
        void contextProc();

    private:
        Task _task;

    private:
        friend class CoroContainer;
        Coro *_nextForContainer;

    private://last
        friend class Context;
        ContextEngine _engine;
    };
}}}

#endif
