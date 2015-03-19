#include "allocator.hpp"
#include "vm.hpp"
#include "system.hpp"
#include "allocator/stack.hpp"

#include "allocator.ipp"
#include "utils/intrusiveDeque.ipp"
#include "utils/sized_cast.ipp"
#include "allocator/bitIndex.ipp"
#include "allocator/bitIndex/level.ipp"
#include "allocator/blocksHolder.ipp"
#include "allocator/block.ipp"

#include <cstdlib>

namespace dci { namespace mm
{
    namespace
    {
        bool g_vmAccessHandler(void *addr)
        {
            return g_allocator.vmAccessHandler(addr);
        }
    }

    void Allocator::init()
    {
        _vm = vm::alloc(_vmSize);

        if(!_vm)
        {
            std::abort();
        }

        std::size_t addr = utils::sized_cast<std::size_t>(_vm);

        _blocksBitIndex = new(utils::sized_cast<void *>(addr)) BlocksBitIndex;
        addr += _blocksBitIndexAlignedSize;

        addr = ConfigHeap::alignUp(addr, ConfigHeap::_blockSize);
        _blocks = utils::sized_cast<void *>(addr);
        addr += _blocksAlignedSize;

        _stacksBitIndex = new(utils::sized_cast<void *>(addr)) StacksBitIndex;
        addr += _stacksBitIndexAlignedSize;

        addr = ConfigHeap::alignUp(addr, ConfigStack::_stackPages*ConfigMemory::_pageSize);
        _stacks = utils::sized_cast<void *>(addr);
        addr += _stacksAlignedSize;

        if(!vm::init(g_vmAccessHandler))
        {
            std::abort();
        }
    }

    void Allocator::deinit()
    {
        assert(_blocksBitIndex);
        if(_blocksBitIndex)
        {
            _blocksBitIndex->~BitIndex();
            _blocksBitIndex = nullptr;
        }
        _blocks = nullptr;

        if(_stacksBitIndex)
        {
            _stacksBitIndex->~BitIndex();
            _stacksBitIndex = nullptr;
        }
        _stacks = nullptr;

        assert(_vm);
        vm::free(_vm, _vmSize);
        _vm = nullptr;

        vm::deinit(&g_vmAccessHandler);
    }

    const dci::mm::Stack *Allocator::stackAlloc()
    {
        allocator::bitIndex::Address addr = _stacksBitIndex->allocate();
        allocator::Stack *inst = utils::sized_cast<allocator::Stack *>(_stacks) + addr;

        new(inst) allocator::Stack;

        return allocator::Stack::impl2Face(inst);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Allocator::stackFree(const dci::mm::Stack *stack)
    {
        allocator::Stack *inst = allocator::Stack::face2Impl(stack);
        allocator::bitIndex::Address addr = inst - utils::sized_cast<allocator::Stack *>(_stacks);

        _stacksBitIndex->deallocate(addr);

        inst->~Stack();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Allocator::stackCompact(const dci::mm::Stack *stack)
    {
        allocator::Stack *inst = allocator::Stack::face2Impl(stack);
        inst->compact();
    }

    bool Allocator::vmAccessHandler(void *addr)
    {
        if(likely(utils::sized_cast<std::size_t>(addr) - utils::sized_cast<std::size_t>(_blocks) < utils::sized_cast<std::size_t>(ConfigHeap::_blocksAmount * ConfigHeap::_blockSize)))
        {
#ifndef NDEBUG
            allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(addr) - utils::sized_cast<char *>(_blocks)) / ConfigHeap::_blockSize;
            assert(_blocksBitIndex->isAllocated(blockBitAddr));
#endif
            void *pagePtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(addr) / ConfigMemory::_pageSize * ConfigMemory::_pageSize);

            if(!vm::protect(pagePtr, ConfigMemory::_pageSize, true))
            {
                std::abort();
                return false;
            }

            return true;
        }
        return false;
    }

    namespace
    {
        std::aligned_storage<sizeof(Allocator), alignof(Allocator)>::type g_allocator_area;
    }
    Allocator &g_allocator(*(Allocator *)(&g_allocator_area));
}}
