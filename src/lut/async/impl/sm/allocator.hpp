#ifndef _LUT_ASYNC_IMPL_SM_ALLOCATOR_HPP_
#define _LUT_ASYNC_IMPL_SM_ALLOCATOR_HPP_

#include "lut/async/impl/sm/stack.hpp"
#include "lut/async/impl/sm/index.hpp"
#include "lut/async/impl/sm/vm.hpp"

namespace lut { namespace async { namespace impl { namespace sm
{
    template <
        size_t levelBittness,
        size_t deepth,
        size_t pageSize,
        size_t maxStackPages,
        size_t initialMappedPages=1,
        size_t unmapBoundBytes=1024,
        bool useGuardPage=true,
        size_t bittnessConcurrency=8>
    class Allocator
            : public vm::User
    {
    public:
        Allocator();
        ~Allocator();

        const Stack *alloc(bool quietFail = false);
        bool free(const Stack *stack);

        bool compact(const Stack *stackOrNull = NULL);

    private:
        virtual bool vmHandleAccess(void *addr);

    private:
        Index<levelBittness, deepth, bittnessConcurrency> _index;

        char *_vspaceBegin;
        char *_vspaceEnd;
    };














    ///////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t deepth, size_t pageSize, size_t maxStackPages, size_t initialMappedPages, size_t unmapBoundBytes, bool useGuardPage, size_t bittnessConcurrency>
    Allocator<levelBittness, deepth,  pageSize, maxStackPages, initialMappedPages, unmapBoundBytes, useGuardPage, bittnessConcurrency>::Allocator()
        : _vspaceBegin(nullptr)
        , _vspaceEnd(nullptr)
    {
        if(!vm::startup(this))
        {
            fprintf(stderr, "unable to startup vm\n");
            return;
        }

        _vspaceBegin = (char *)vm::alloc(_index.volume*pageSize*maxStackPages);

        if(!_vspaceBegin)
        {
            _vspaceBegin = nullptr;
            return;
        }

        _vspaceEnd = _vspaceBegin + _index.volume*pageSize*maxStackPages;
    }

    ///////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t deepth, size_t pageSize, size_t maxStackPages, size_t initialMappedPages, size_t unmapBoundBytes, bool useGuardPage, size_t bittnessConcurrency>
    Allocator<levelBittness, deepth,  pageSize, maxStackPages, initialMappedPages, unmapBoundBytes, useGuardPage, bittnessConcurrency>::~Allocator()
    {
        vm::shutdown(this);

        if(_vspaceBegin)
        {
            vm::free(_vspaceBegin, _index.volume*pageSize);
            _vspaceBegin = nullptr;
            _vspaceEnd = nullptr;
        }
    }

    ///////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t deepth, size_t pageSize, size_t maxStackPages, size_t initialMappedPages, size_t unmapBoundBytes, bool useGuardPage, size_t bittnessConcurrency>
    const Stack *Allocator<levelBittness, deepth,  pageSize, maxStackPages, initialMappedPages, unmapBoundBytes, useGuardPage, bittnessConcurrency>::alloc(bool quietFail)
    {
        size_t iaddress = _index.alloc();
        if((size_t)-1 == iaddress)
        {
            if(!quietFail)
            {
                fprintf(stderr, "unable to allocate stack\n");
            }
            return nullptr;
        }

        size_t size = maxStackPages*pageSize;
        size_t initialMappedSize = std::max(initialMappedPages, (size_t)1) * pageSize;

        char *begin = _vspaceBegin + iaddress*size;
        char *end = begin+size;

        if(!vm::protect(end-initialMappedSize, initialMappedSize, true))
        {
            _index.free(iaddress);

            if(!quietFail)
            {
                fprintf(stderr, "unable to protect vm\n");
            }
            return nullptr;
        }

        Stack *stack = reinterpret_cast<Stack *>(end - sizeof(Stack));

        stack->_guardBound = useGuardPage ? (begin + pageSize) : begin;

        stack->_mappedBound = end - initialMappedSize;
        stack->_userspaceBound = end - sizeof(Stack);


        return stack;
    }

    ///////////////////////////////////////////////////////////
    template <size_t levelBittness, size_t deepth, size_t pageSize, size_t maxStackPages, size_t initialMappedPages, size_t unmapBoundBytes, bool useGuardPage, size_t bittnessConcurrency>
    bool Allocator<levelBittness, deepth,  pageSize, maxStackPages, initialMappedPages, unmapBoundBytes, useGuardPage, bittnessConcurrency>::free(const Stack *stack)
    {
        if((char *)stack < _vspaceBegin || (char *)stack >= _vspaceEnd)
        {
            return false;
        }

        size_t size = maxStackPages*pageSize;
        size_t iaddress = ((char *)stack - _vspaceBegin) / size;

        if(_index.free(iaddress))
        {
            char *begin = _vspaceBegin + iaddress*size;

            if(!vm::protect(begin, size, false))
            {
                fprintf(stderr, "unable to unprotect stack area\n");
                return true;
            }

            return true;
        }

        return false;
    }

    template <size_t levelBittness, size_t deepth, size_t pageSize, size_t maxStackPages, size_t initialMappedPages, size_t unmapBoundBytes, bool useGuardPage, size_t bittnessConcurrency>
    bool Allocator<levelBittness, deepth,  pageSize, maxStackPages, initialMappedPages, unmapBoundBytes, useGuardPage, bittnessConcurrency>::compact(const Stack *stackOrNull)
    {
        char *addr = stackOrNull ? (char *)stackOrNull : (char *)alloca(1);

        if(addr < _vspaceBegin || addr >= _vspaceEnd)
        {
            return false;
        }

        size_t size = maxStackPages*pageSize;
        size_t iaddress = (addr - _vspaceBegin) / size;

        if(_index.isAllocated(iaddress))
        {
            char *begin = _vspaceBegin + iaddress*size;
            char *end = begin+size;
            Stack *stack = reinterpret_cast<Stack *>(end - sizeof(Stack));
            assert(!stackOrNull || stackOrNull == stack);

            size_t initialMappedSize = std::max(initialMappedPages, (size_t)1) * pageSize;

            addr -= unmapBoundBytes;

            if(addr >= stack->_guardBound && addr < end - initialMappedSize)
            {
                char *newMappedBound = (char *)((intptr_t)addr & ~(pageSize-1));

                if(newMappedBound > stack->_mappedBound)
                {
                    if(vm::protect(stack->_mappedBound, newMappedBound - stack->_mappedBound, false))
                    {
                        stack->_mappedBound = newMappedBound;
                    }
                    else
                    {
                        fprintf(stderr, "unable to unprotect vm during stack compaction: %zu, stack area: %p - %p, addr %p\n", iaddress, begin, end, addr);
                        return false;
                    }
                    return true;
                }
            }

            return true;
        }

        return false;
    }

    template <size_t levelBittness, size_t deepth, size_t pageSize, size_t maxStackPages, size_t initialMappedPages, size_t unmapBoundBytes, bool useGuardPage, size_t bittnessConcurrency>
    bool Allocator<levelBittness, deepth,  pageSize, maxStackPages, initialMappedPages, unmapBoundBytes, useGuardPage, bittnessConcurrency>::vmHandleAccess(void *addr)
    {
        if(addr < _vspaceBegin || addr >= _vspaceEnd)
        {
            return false;
        }

        size_t size = maxStackPages*pageSize;
        size_t iaddress = ((char *)addr - _vspaceBegin) / size;

        if(_index.isAllocated(iaddress))
        {
            char *begin = _vspaceBegin + iaddress*size;
            char *end = begin+size;
            Stack *stack = reinterpret_cast<Stack *>(end - sizeof(Stack));

            if(addr >= stack->_guardBound)
            {
                char * newMappedBound = (char *)((intptr_t)addr & ~(pageSize-1));
                if(vm::protect(newMappedBound, stack->_mappedBound - newMappedBound, true))
                {
                    stack->_mappedBound = newMappedBound;
                }
                else
                {
                    fprintf(stderr, "unable to protect vm during stack access request:  %zu, stack area: %p - %p, addr %p\n", iaddress, begin, end, addr);
                }
                return true;
            }

            fprintf(stderr, "stack overflow, iaddress: %zu, stack area: %p - %p, addr %p\n", iaddress, begin, end, addr);
            return false;
        }

        return false;
    }

}}}}

#endif
