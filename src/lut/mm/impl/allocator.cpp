#include "lut/async/stable.hpp"
#include "lut/mm/impl/allocator.hpp"
#include "lut/mm/impl/vm.hpp"
#include "lut/mm/config.hpp"

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Allocator()
    {
        vm::protect(&_headerArea, sizeof(HeaderArea), true);
        new(&_headerArea) Header;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::~Allocator()
    {
        assert(!header()._threadsUseMask.count());
        header().~Header();
        vm::protect(&_headerArea, sizeof(HeaderArea), false);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator &Allocator::instance()
    {
        return *_instance;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Allocator::threadInit()
    {
        Thread *t;
        std::size_t tIndex;
        {
            std::unique_lock<std::mutex> l(header()._mtx);

            if(header()._threadsUseMask.count() >= Config::_maxThreadsAmount)
            {
                return false;
            }

            tIndex = 0;
            for(; tIndex<header()._threadsUseMask.size(); ++tIndex)
            {
                if(!header()._threadsUseMask.test(tIndex))
                {
                    break;
                }
            }

            header()._threadsUseMask.set(tIndex);

            t = thread(tIndex);
            t = new(t) Thread;
        }

        if(!vm::threadInit(&Allocator::s_vmAccessHandler))
        {
            std::unique_lock<std::mutex> l(header()._mtx);

            header()._threadsUseMask.reset(tIndex);
            t->~Thread();

            return false;
        }

        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Allocator::threadDeinit()
    {
        Thread *t = &Thread::instance();
        if(!t)
        {
            return false;
        }

        std::size_t tIndex = threadIndex(t);
        if(_badIndex == tIndex)
        {
            return false;
        }

        {
            std::unique_lock<std::mutex> l(header()._mtx);

            if(!header()._threadsUseMask.test(tIndex))
            {
                return false;
            }

            header()._threadsUseMask.reset(tIndex);
            t->~Thread();
        }

        return vm::threadDeinit(&Allocator::s_vmAccessHandler);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const lut::mm::Stack *Allocator::stackAlloc()
    {
        return Thread::instance().stackAlloc();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Allocator::stackFree(const lut::mm::Stack *stack)
    {
        Thread *t = thread(stack);
        assert(t);
        if(!t)
        {
            return;
        }

        if(t == &Thread::instance())
        {
            return t->stackFree(stack);
        }
        return t->stackFreeFromOverThread(stack);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Allocator::stackCompact(const lut::mm::Stack *stack)
    {
        Thread *t = thread(stack);
        assert(t);
        if(!t)
        {
            return;
        }

        return t->stackCompact(stack);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Allocator::vmAccessHandler(void *addr)
    {
        Thread *t = thread(addr);
        assert(t);
        if(!t)
        {
            return false;
        }

        return t->vmAccessHandler(reinterpret_cast<std::uintptr_t>(addr) - reinterpret_cast<std::uintptr_t>(t));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Allocator::s_vmAccessHandler(void *addr)
    {
        return instance().vmAccessHandler(addr);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Header &Allocator::header()
    {
        return *reinterpret_cast<Header *>(&_headerArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::size_t Allocator::threadIndex(const void *addr)
    {
        if(addr < (const char*)&_threadsArea || addr > (const char*)&_threadsArea + sizeof(ThreadsArea))
        {
            return _badIndex;
        }

        return (reinterpret_cast<const char *>(addr) - (const char*)&_threadsArea) / sizeof(Thread);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread *Allocator::thread(const void *addr)
    {
        std::size_t index = threadIndex(addr);

        if(_badIndex == index)
        {
            return nullptr;
        }

        return thread(index);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Thread *Allocator::thread(std::size_t index)
    {
        assert(_badIndex != index);
        return reinterpret_cast<Thread *>(&_threadsArea) + index;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator *Allocator::_instance;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Instantiator Allocator::_instantiator {};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Allocator::Instantiator::Instantiator()
    {
        void *ptr = vm::alloc(sizeof(Allocator));
        Allocator::_instance = new(ptr) Allocator;
    }

    Allocator::Instantiator::~Instantiator()
    {
        Allocator::_instance->~Allocator();

        vm::free(Allocator::_instance, sizeof(Allocator));
        Allocator::_instance = nullptr;
    }

}}}
