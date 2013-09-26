#ifndef _LUT_ASYNC_IMPL_TASK_HPP_
#define _LUT_ASYNC_IMPL_TASK_HPP_

#include <functional>

namespace lut { namespace async { namespace impl
{
    class Task
    {
        Task(const Task &) = delete;
        void operator=(const Task &) = delete;

    public:
        Task();
        template<class F, class... Args>
        Task(F &&f, Args &&...args);

        ~Task();

        void moveTo(Task &&task);
        bool empty() const;
        void clear();

        void exec();

    private:

        struct CallHolderBase
        {
            virtual ~CallHolderBase() {}
            virtual void call() = 0;
            virtual void moveTo(CallHolderBase *) = 0;
        };

        template <class F, class... Args>
        class CallHolder
            : public Task::CallHolderBase
        {
        public:
            CallHolder(F &&f, Args &&...args)
                : _bind(std::forward<F>(f), std::forward<Args>(args)...)
            {
            }

            CallHolder(CallHolder &&from)
                : _bind(std::forward<Bind>(from._bind))
            {
            }

            virtual ~CallHolder() override
            {
            }

            virtual void call() override
            {
                _bind();
            }

            virtual void moveTo(CallHolderBase *p) override
            {
                new(p) CallHolder(std::forward<CallHolder>(*this));
            }

        private:
            using Bind = decltype(std::bind(std::declval<F>(), std::declval<Args>()...));
            Bind _bind;
        };

        union
        {
            char _buffer4CallHolder[60];
            CallHolderBase *_callHolderPointer;
        } _data;
        uint32_t _size;//0, <=60, >60

        bool isInternalPlaced();
        CallHolderBase *callHolderBase();
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<class F, class... Args>
    Task::Task(F &&f, Args &&...args)
    {
        using CallHolder = CallHolder<F, Args...>;

        _size = sizeof(CallHolder);
        if(isInternalPlaced())
        {
            new(callHolderBase()) CallHolder(std::forward<F>(f), std::forward<Args>(args)...);
        }
        else
        {
            _data._callHolderPointer = new CallHolder(std::forward<F>(f), std::forward<Args>(args)...);
        }
    }

}}}

#endif
