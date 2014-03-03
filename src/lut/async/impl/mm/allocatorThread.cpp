#include "lut/async/stable.hpp"
#include "lut/async/impl/mm/allocatorThread.hpp"

namespace lut { namespace async { namespace impl { namespace mm
{
    AllocatorThread::AllocatorThread()
    {
        assert(!_instance);
        _instance = this;
    }

    AllocatorThread &AllocatorThread::instance()
    {
        return *_instance;
    }

    AllocatorThread::~AllocatorThread()
    {
        assert(this == _instance);
        _instance = nullptr;
    }

    std::size_t AllocatorThread::vspaceSize()
    {
        return vspaceHeaderSize() + 4096;
    }

    std::size_t AllocatorThread::vspaceHeaderSize()
    {
        return
                sizeof(AllocatorThread) % _config.pageSize() ?
                    (sizeof(AllocatorThread) / _config.pageSize() + 1) * _config.pageSize() :
                    sizeof(AllocatorThread);
    }

    char *AllocatorThread::vspaceBegin() const
    {
        return _vspaceBegin;
    }

    char *AllocatorThread::vspaceEnd() const
    {
        return _vspaceEnd;
    }

    const Stack *AllocatorThread::stackAlloc()
    {
        assert(0);
    }

    bool AllocatorThread::stackFree(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorThread::stackFreeFromOverThread(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorThread::stackCompact(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorThread::vmAccessHandler(void *addr)
    {
        assert(0);
    }

    __thread AllocatorThread *AllocatorThread::_instance{nullptr};
    const Config &AllocatorThread::_config{Config::instance()};


}}}}
