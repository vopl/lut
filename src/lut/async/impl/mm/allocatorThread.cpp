#include "lut/async/stable.hpp"
#include "lut/async/impl/mm/allocatorThread.hpp"

namespace lut { namespace async { namespace impl { namespace mm
{
    AllocatorThread::AllocatorThread()
        : _vspaceBegin(reinterpret_cast<char *>(this))
        , _vspaceStacksBegin(_vspaceBegin + vspaceHeaderSize())
        , _vspaceStacksEnd(_vspaceStacksBegin + vspaceStacksSize())
        , _vspaceBuffersBegin(_vspaceStacksEnd)
        , _vspaceBuffersEnd(_vspaceBuffersBegin + vspaceBuffersSize())
        , _vspaceEnd(_vspaceBegin + vspaceSize())
    {
        assert(!_instance);
        _instance = this;

        assert(_vspaceBuffersEnd == _vspaceEnd);
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
        return
                vspaceHeaderSize() +
                vspaceStacksSize() +
                vspaceBuffersSize();
    }

    std::size_t AllocatorThread::vspaceHeaderSize()
    {
        return
                sizeof(AllocatorThread) % _config.pageSize() ?
                    (sizeof(AllocatorThread) / _config.pageSize() + 1) * _config.pageSize() :
                    sizeof(AllocatorThread);
    }

    std::size_t AllocatorThread::vspaceStacksSize()
    {
        return _config.stackPages() * _config.stacksAmount() * _config.pageSize();
    }

    std::size_t AllocatorThread::vspaceBuffersSize()
    {
        return _config.blockPages() * _config.blocksAmount() * _config.pageSize();
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
