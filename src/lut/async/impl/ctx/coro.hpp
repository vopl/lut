#ifndef _LUT_ASYNC_IMPL_CTX_CORO_HPP_
#define _LUT_ASYNC_IMPL_CTX_CORO_HPP_

#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/task.hpp"
#include "lut/mm/stack.hpp"

namespace lut { namespace async { namespace impl
{
    class Scheduler;

    template <typename T>
    class EffortContainer;

}}}

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Coro
        : public Engine
    {
        Coro &operator=(const Coro &) = delete;

        Coro(Scheduler *scheduler, const lut::mm::Stack *stack);
        ~Coro();

    public:
        static Coro *alloc(Scheduler *scheduler);
        void free();

        void setCode(Task *task);

    public:
        void switchTo(Engine *to);

    private:
        static void s_contextProc(std::intptr_t self);
        void contextProc();

    private:
        Scheduler *             _scheduler;
        const lut::mm::Stack *  _stack;
        Task *                  _task;

    private:
        friend class ::lut::async::impl::EffortContainer<Coro>;
        Coro *  _nextInList;
    };
}}}}

#endif
