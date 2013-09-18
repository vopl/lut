#ifndef _ASYNC_CODEMANAGER_HPP_
#define _ASYNC_CODEMANAGER_HPP_

#include <memory>

namespace async
{

    class Scheduler;

    namespace impl
    {
        class Scheduler;
        typedef std::shared_ptr<Scheduler> SchedulerPtr;
    }

    class CodeManager
    {
    public:
        CodeManager();
        CodeManager(const impl::SchedulerPtr &implScheduler);

        operator Scheduler();

        void spawn(const std::function<void()> &code);
        void spawn(std::function<void()> &&code);

    private:
        impl::SchedulerPtr _implScheduler;
    };
}

#endif
