#ifndef _LUT_ASYNC_IMPL_CORO_HPP_
#define _LUT_ASYNC_IMPL_CORO_HPP_

#include "lut/async/impl/context.hpp"

#include <functional>

namespace lut { namespace async { namespace impl
{

    class Coro
        : public IntrusiveQueue<Coro>::ElementBase
    {
        Coro &operator=(const Coro &) = delete;

    public:
        using Task = std::function<void()>;

    public:
        static Coro *make(size_t stackSize = 1024*32);
        Coro();
        void operator delete(void *p);
        ~Coro();

        void setCode(const Task &code);
        void setCode(Task &&code);

        void switchTo(Context *to);
        void switchTo(Coro *to);

    private:
        static void s_contextProc(intptr_t self);
        void contextProc();

    private:
        Task _code;

    private://last
        friend class Context;
        ContextEngine _engine;
    };
}}}

#endif
