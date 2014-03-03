#include "lut/async/stable.hpp"
#include "lut/async/impl/mm/allocatorGeneral.hpp"
#include "lut/async/impl/mm/vm.hpp"
#include "lut/async/impl/mm/config.hpp"

namespace lut { namespace async { namespace impl { namespace mm
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    AllocatorGeneral::AllocatorGeneral()
        : _vspaceBegin(nullptr)
        , _vspaceEnd(nullptr)
    {
        if(_config.maxThreadsAmount() > sizeof(Header::_threadsUseMask)*8)
        {
            throw std::runtime_error("config.maxThreadsAmount too big");
        }

        _vspaceBegin = (char *)vm::alloc(vspaceSize());

        if(!_vspaceBegin)
        {
            _vspaceBegin = nullptr;
            return;
        }

        vm::protect(_vspaceBegin, vspaceHeaderSize(), true);
        _header = new(_vspaceBegin) Header;

        _header->_threadsArea = _vspaceBegin + vspaceHeaderSize();

        _vspaceEnd = _vspaceBegin + vspaceSize();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    AllocatorGeneral &AllocatorGeneral::instance()
    {
        return _instance;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    AllocatorGeneral::~AllocatorGeneral()
    {
        if(_header)
        {
            assert(!_header->_threadsUseMask.count());
           _header->~Header();
           vm::protect(_vspaceBegin, vspaceHeaderSize(), false);
        }

        if(_vspaceBegin)
        {
            vm::free(_vspaceBegin, vspaceSize());
            _vspaceBegin = nullptr;
            _vspaceEnd = nullptr;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool AllocatorGeneral::threadInit()
    {
        AllocatorThread *at;
        std::size_t atIndex;
        {
            std::unique_lock<std::mutex> l(_mtx);

            if(_header->_threadsUseMask.count() >= _config.maxThreadsAmount())
            {
                return false;
            }

            atIndex = 0;
            for(; atIndex<_header->_threadsUseMask.size(); ++atIndex)
            {
                if(!_header->_threadsUseMask.test(atIndex))
                {
                    break;
                }
            }

            _header->_threadsUseMask.set(atIndex);

            at = allocatorThread(atIndex);
            vm::protect(at, vspaceHeaderSize(), true);
            at = new(at) AllocatorThread;
        }

        if(!vm::threadInit(&AllocatorGeneral::s_vmAccessHandler))
        {
            std::unique_lock<std::mutex> l(_mtx);

            _header->_threadsUseMask.reset(atIndex);
            at->~AllocatorThread();
            vm::protect(at, vspaceHeaderSize(), false);

            return false;
        }

        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool AllocatorGeneral::threadDeinit()
    {
        AllocatorThread *at = &AllocatorThread::instance();
        if(!at)
        {
            return false;
        }

        std::size_t atIndex = allocatorThreadIndex(at);
        if(_badIndex == atIndex)
        {
            return false;
        }

        {
            std::unique_lock<std::mutex> l(_mtx);

            if(!_header->_threadsUseMask.test(atIndex))
            {
                return false;
            }

            _header->_threadsUseMask.reset(atIndex);
            at->~AllocatorThread();
            vm::protect(at, vspaceHeaderSize(), false);
        }

        return vm::threadDeinit(&AllocatorGeneral::s_vmAccessHandler);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const Stack *AllocatorGeneral::stackAlloc()
    {
        return AllocatorThread::instance().stackAlloc();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool AllocatorGeneral::stackFree(const Stack *stack)
    {
        AllocatorThread *at = allocatorThread(stack);
        assert(at);
        if(!at)
        {
            return false;
        }

        if(at == &AllocatorThread::instance())
        {
            return at->stackFree(stack);
        }
        return at->stackFreeFromOverThread(stack);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool AllocatorGeneral::stackCompact(const Stack *stack)
    {
        AllocatorThread *at = allocatorThread(stack);
        assert(at);
        if(!at)
        {
            return false;
        }

        return at->stackCompact(stack);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::size_t AllocatorGeneral::vspaceSize()
    {
        return vspaceHeaderSize() + AllocatorThread::vspaceSize() * _config.maxThreadsAmount();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::size_t AllocatorGeneral::vspaceHeaderSize()
    {
        return
                sizeof(Header) % _config.pageSize() ?
                    (sizeof(Header) / _config.pageSize() + 1) * _config.pageSize() :
                    sizeof(Header);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    char *AllocatorGeneral::vspaceBegin() const
    {
        return _vspaceBegin;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    char *AllocatorGeneral::vspaceEnd() const
    {
        return _vspaceEnd;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool AllocatorGeneral::vmAccessHandler(void *addr)
    {
        AllocatorThread *at = allocatorThread(addr);
        assert(at);
        if(!at)
        {
            return false;
        }

        return at->vmAccessHandler(addr);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool AllocatorGeneral::s_vmAccessHandler(void *addr)
    {
        return instance().vmAccessHandler(addr);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::size_t AllocatorGeneral::allocatorThreadIndex(const void *addr)
    {
        if(addr < _header->_threadsArea || addr > _header->_threadsArea + _config.maxThreadsAmount()*AllocatorThread::vspaceSize())
        {
            return _badIndex;
        }

        return (reinterpret_cast<const char *>(addr) - _header->_threadsArea) / AllocatorThread::vspaceSize();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    AllocatorThread *AllocatorGeneral::allocatorThread(const void *addr)
    {
        std::size_t index = allocatorThreadIndex(addr);

        if(_badIndex == index)
        {
            return nullptr;
        }

        return allocatorThread(index);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    AllocatorThread *AllocatorGeneral::allocatorThread(size_t index)
    {
        assert(_badIndex != index);
        return reinterpret_cast<AllocatorThread *>(_header->_threadsArea + index * AllocatorThread::vspaceSize());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    AllocatorGeneral AllocatorGeneral::_instance{};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const Config &AllocatorGeneral::_config{Config::instance()};

}}}}


