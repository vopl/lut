#ifndef _LUT_ASYNC_IMPL_TASK_HPP_
#define _LUT_ASYNC_IMPL_TASK_HPP_

namespace lut { namespace async { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
    class Task
    {
        Task(const Task &) = delete;
        void operator=(const Task &) = delete;

    protected:
        Task();
        ~Task();

    public:
        virtual void free() = 0;
        virtual void exec() = 0;
    };

}}}

#endif
