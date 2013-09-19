#include "lut/async/stable.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{
    Thread::Thread(Scheduler &scheduler, ThreadState *stateEvt)
        : _scheduler(scheduler)
        , _stateEvt(stateEvt)
        , _mtx()
        , _cv()
        , _releaseRequest()
    {
    }

    Thread::~Thread()
    {

    }


}}}
