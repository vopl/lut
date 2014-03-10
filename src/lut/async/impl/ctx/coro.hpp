#ifndef _LUT_ASYNC_IMPL_CTX_CORO_HPP_
#define _LUT_ASYNC_IMPL_CTX_CORO_HPP_

#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/task.hpp"
#include "lut/mm/impl/stack.hpp"

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Coro
        : public Engine
    {
        Coro &operator=(const Coro &) = delete;

        Coro(const lut::mm::Stack *stack);
        ~Coro();

    public:
        static Coro *alloc();
        void free();

        void setCode(Task *task);

    public:
        void switchTo(Engine *to);

    private:
        static void s_contextProc(intptr_t self);
        void contextProc();

    private:
        const lut::mm::Stack *_stack;
        Task *_task;
    };
}}}}

#endif
