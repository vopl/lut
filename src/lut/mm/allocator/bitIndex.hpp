#pragma once

#include "lut/mm/allocator/bitIndex/level.hpp"
#include "lut/mm/allocator/bitIndex/orderEvaluator.hpp"
#include "lut/mm/config.hpp"

namespace lut { namespace mm { namespace allocator
{
    template <std::size_t volume>
    class BitIndex
    {
    public:
        BitIndex();
        ~BitIndex();

        bitIndex::Address allocate();
        bool isAllocated(bitIndex::Address address);
        void deallocate(bitIndex::Address address);

    private:
        void updateProtection(bitIndex::Address addr);
        void updateProtection(void *addr);

        static constexpr std::size_t _order = bitIndex::OrderEvaluator<volume>::_order;

        using TopLevel = bitIndex::Level<_order, ConfigMemory::_cacheLineSize>;

        struct Header
        {
            std::size_t _protectedSize;
            bitIndex::Address _maxAllocatedAddress;

        } _header;

        char _pad[ConfigMemory::_cacheLineSize - sizeof(Header)];

        TopLevel _topLevel;
    };
}}}
