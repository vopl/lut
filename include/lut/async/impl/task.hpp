#ifndef _LUT_ASYNC_IMPL_TASK_HPP_
#define _LUT_ASYNC_IMPL_TASK_HPP_

#include <thread>

namespace lut { namespace async { namespace impl
{
    namespace ctx
    {
        class Coro;
    }

#pragma pack(push)
#pragma pack(1)
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Task
    {
        Task(const Task &) = delete;
        void operator=(const Task &) = delete;

    protected:
        Task();
        ~Task();

    public:
        virtual void free() = 0;
        virtual void call() = 0;

    protected:
        ctx::Coro *_coro;

        std::thread::id _preferedThreadId;

        uint8_t _priority:4;
        uint8_t _linkToThread:1;

        uint8_t _locksAmount;

        class Lock;
        using LockPtr = Lock*;

        //последний переменной длины по _locksAmount
        LockPtr _locks[1];
    };
#pragma pack(pop)

}}}

#endif
