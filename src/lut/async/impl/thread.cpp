#include "lut/async/stable.hpp"
#include "lut/async/impl/thread.hpp"

namespace lut { namespace async { namespace impl
{
    Thread::Thread(ThreadContainer &threadContainer, ThreadState *stateEvt)
        : _threadContainer(threadContainer)
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
