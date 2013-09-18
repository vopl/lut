#ifndef _ASYNC_THREADSTATE_HPP_
#define _ASYNC_THREADSTATE_HPP_

#include <mutex>
#include <condition_variable>

namespace async
{

    namespace impl
    {
        class Thread;
    }

    class ThreadState
    {
    public:
        enum EValue
        {
            init,
            inited,
            inWork,
            doneWork,
            deinit,
            deinited
        };

    public:
        ThreadState();
        ~ThreadState();

        EValue get();
        void wait(EValue v);
        void waitNot(EValue v);

    private:
        friend class impl::Thread;
        void set(EValue v);

    private:
        std::mutex _mtx;
        std::condition_variable _cv;
        EValue _value;
    };
}

#endif
