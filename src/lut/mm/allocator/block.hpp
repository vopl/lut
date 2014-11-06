#pragma once

#include "lut/mm/utils/intrusiveDeque.hpp"
#include "lut/mm/config.hpp"

namespace lut { namespace mm { namespace allocator
{
    ////////////////////////////////////////////////////////////////////////////////
    class BlocksHolder;

    ////////////////////////////////////////////////////////////////////////////////
    class Block
    {
    public:
        static Block *allocateBlock(BlocksHolder *thisBlockHolder, std::size_t sizeClass);
        static void deallocateBlock(Block *block);

    public:
        Block(BlocksHolder *thisBlockHolder, std::size_t sizeClass);
        ~Block();


        template <std::size_t sizeClass> void *alloc();
        template <std::size_t sizeClass> void free(void *ptr);

        void *alloc();
        void free(void *ptr);

        bool empty() const;
        bool full() const;

        std::size_t getSizeClass() const;

    private:
        void *allocWithoutCounters();
        void freeWithoutCounters(void *ptr);

    private:
         using Counter = std::uint64_t;
         struct ElementHeader
         {
             ElementHeader *_next;
         };

    private:
        ElementHeader *_nextFreeElement;

        //for intrusiveDeque
        template <typename T> friend class utils::IntrusiveDeque;

        Block *_next;
        Block *_prev;

    private:
        BlocksHolder *_thisBlockHolder;
        Counter _allocated;
        std::size_t _sizeClass;
        std::size_t _elementsAmount;

        alignas(ConfigMemory::_cacheLineSize) char _elementsArea[ConfigHeap::_blockSize - ConfigMemory::_cacheLineSize];
    };


    ////////////////////////////////////////////////////////////////////////////////
    static_assert(ConfigHeap::_blockSize == sizeof(Block), "Block must be ConfigHeap::_blockSize bytes");

}}}
