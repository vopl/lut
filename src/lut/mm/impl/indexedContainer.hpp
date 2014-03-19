#ifndef _LUT_MM_IMPL_INDEXEDCONTAINER_HPP_
#define _LUT_MM_IMPL_INDEXEDCONTAINER_HPP_

#include "lut/mm/config.hpp"
#include "lut/mm/impl/bitIndex.hpp"
#include "lut/mm/impl/utils.hpp"
#include "lut/mm/impl/branchHint.hpp"

#include <type_traits>

namespace lut { namespace mm { namespace impl
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    class IndexedContainer
    {

        static_assert(sizeof(Buffer) == sizeof(Buffer)/Config::_pageSize*Config::_pageSize, "sizeof Buffer must be pageSize aligned");

    public:
        IndexedContainer();
        ~IndexedContainer();

        Buffer *alloc();
        void free(Buffer *ptr);

        template <typename DerivedBuffer> DerivedBuffer *alloc();
        template <typename DerivedBuffer> void free(DerivedBuffer *ptr);

        template <typename DerivedBuffer> DerivedBuffer *bufferByPointer(const void *ptr);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
        using Index = BitIndex<amount>;
        using IndexArea = typename std::aligned_storage<sizeof(Index), Config::_pageSize>::type;

        IndexArea _indexArea;
        Index &index();

    private:
        using BuffersArea = typename std::aligned_storage<sizeof(Buffer)*amount, Config::_pageSize>::type;

        BuffersArea _buffersArea;
        Buffer *buffers();
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    IndexedContainer<Buffer, amount>::IndexedContainer()
    {
        new (&index()) Index;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    IndexedContainer<Buffer, amount>::~IndexedContainer()
    {
        index().~Index();
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    Buffer *IndexedContainer<Buffer, amount>::alloc()
    {
        AddressInIndex addr = index().alloc();
        Buffer *ptr = buffers() + addr;
        new (ptr) Buffer;
        return ptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    void IndexedContainer<Buffer, amount>::free(Buffer *ptr)
    {
        AddressInIndex addr = ptr - buffers();
        index().free(addr);
        ptr->~Buffer();
    }









    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    template <typename DerivedBuffer>
    DerivedBuffer *IndexedContainer<Buffer, amount>::alloc()
    {
        static_assert(sizeof(Buffer) == sizeof(DerivedBuffer), "derived and base must have same layout");
        static_assert(std::is_base_of<Buffer, DerivedBuffer>::value, "derivedBuffer must be inherited from buffer");

        AddressInIndex addr = index().alloc();
        if(unlikely(badAddressInIndex == addr))
        {
            assert(!"no space");
            fprintf(stderr, "no space left in mm\n");
            return 0;
        }
        DerivedBuffer *ptr = static_cast<DerivedBuffer *>(buffers() + addr);
        new (ptr) DerivedBuffer;
        return ptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    template <typename DerivedBuffer>
    void IndexedContainer<Buffer, amount>::free(DerivedBuffer *ptr)
    {
        static_assert(sizeof(Buffer) == sizeof(DerivedBuffer), "derived and base must have same layout");
        static_assert(std::is_base_of<Buffer, DerivedBuffer>::value, "derivedBuffer must be inherited from buffer");

        AddressInIndex addr = ptr - buffers();
        index().free(addr);
        ptr->~DerivedBuffer();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    template <typename DerivedBuffer>
    DerivedBuffer *IndexedContainer<Buffer, amount>::bufferByPointer(const void *ptr)
    {
        std::uintptr_t buffersPointer = reinterpret_cast<std::uintptr_t>(&_buffersArea);
        AddressInIndex bufferAddr = (reinterpret_cast<std::uintptr_t>(ptr) - buffersPointer) / sizeof(Buffer);

        assert(bufferAddr < amount);
        assert(index().isAllocated(bufferAddr));

        return static_cast<DerivedBuffer *>(reinterpret_cast<Buffer *>(buffersPointer) + bufferAddr);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    bool IndexedContainer<Buffer, amount>::vmAccessHandler(std::uintptr_t offset)
    {
        assert(offset < sizeof(IndexedContainer));

        if(offset < offsetof(IndexedContainer, _buffersArea))
        {
            return index().vmAccessHandler(offset - offsetof(IndexedContainer, _indexArea));
        }

        AddressInIndex bufferAddr = (offset - offsetof(IndexedContainer, _buffersArea)) / sizeof(Buffer);

        if(index().isAllocated(bufferAddr))
        {
            Buffer *buffer = buffers() + bufferAddr;
            return buffer->vmAccessHandler(offset - offsetof(IndexedContainer, _buffersArea) - bufferAddr * sizeof(Buffer));
        }

        assert(!"seg fault in unallocated buffer");
        return false;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    typename IndexedContainer<Buffer, amount>::Index &IndexedContainer<Buffer, amount>::index()
    {
        return *reinterpret_cast<Index *>(&_indexArea);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename Buffer, std::size_t amount>
    Buffer *IndexedContainer<Buffer, amount>::buffers()
    {
        return reinterpret_cast<Buffer *>(&_buffersArea);
    }
}}}

#endif
