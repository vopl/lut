#include <atomic>
#include <thread>
#include <cassert>

namespace test
{
    template <uint32_t sizeofBlock, uint32_t blocksAmount>
    class Pool_c
    {
    public:

        using Offset = uint32_t;
        static_assert(sizeof(Offset) <= sizeofBlock, "sizeof(Index) <= sizeofBlock");

        union Block
        {
            Offset _next;
            struct
            {
                Offset _next;
                Offset _last;
                Offset _size;
            } _freeAwaitListMember;

            char _stub[sizeofBlock];
        };

        static_assert(sizeof(Block) == sizeofBlock, "sizeof(Block) == sizeofBlock");

        Offset _next;
        Offset _allocated;

        std::atomic<Offset> _atomicFreeAwaitList;
        static const Offset _badOffset = (Offset)-1;

        std::thread::id _assignedThreadId;
        static __thread std::thread::native_handle_type _threadId;

        char _blocks[blocksAmount*sizeofBlock];

    public:

        Pool_c()
            : _next{0}
            , _allocated(0)
            , _atomicFreeAwaitList{_badOffset}
            , _assignedThreadId()
        {
        }

        ~Pool_c()
        {
            //assert(m_next == m_memStart);
        }

        Block* Offset2Block(Offset i)
        {
            return reinterpret_cast<Block*>(_blocks + i);
        }

        Offset Block2Offset(Block* p)
        {
            return reinterpret_cast<char *>(p) - _blocks;
        }









        void *alloc()
        {
            Offset freeAwaitList = _atomicFreeAwaitList.load(std::memory_order_acquire);
            if(_badOffset != freeAwaitList)
            {
                freeAwaitList = _atomicFreeAwaitList.exchange(_badOffset, std::memory_order_release);
                assert(_badOffset != freeAwaitList);

                Block *begin = Offset2Block(freeAwaitList);
                if(1 == begin->_freeAwaitListMember._size)
                {
                    assert(begin->_freeAwaitListMember._last == freeAwaitList);
                    return begin;
                }

                Block *last = Offset2Block(begin->_freeAwaitListMember._last);

                last->_next = _next;
                _next = begin->_next;
                _allocated -= begin->_freeAwaitListMember._size - sizeofBlock;

                return begin;
            }

            assert(_next <= blocksAmount*sizeof(Block));
            assert(_allocated <= blocksAmount*sizeof(Block));

            if(_next >= blocksAmount*sizeofBlock)
            {
                return nullptr;
            }

            Block *block = Offset2Block(_next);

            if(_next == _allocated)
            {
                _allocated = _next = _next+sizeofBlock;
                if(_allocated >= blocksAmount*sizeofBlock)
                {
                    _allocated |= 0x80000000U;
                }
            }
            else
            {
                _next = block->_next;
                _allocated += sizeofBlock;
            }

            return block;
        }

        void free(void *p)
        {
            assert(p >= _blocks && p < (_blocks+blocksAmount*sizeofBlock));
            assert(!(((char *)p - (char *)_blocks) % sizeof(Block)));

            union
            {
                void *_ptr;
                Block *_block;

            } cnvt;
            cnvt._ptr = p;
            Block *block = cnvt._block;

            Offset blockOffset = Block2Offset(block);

            if(_assignedThreadId == std::thread::id(_threadId))
            {
                block->_next = _next;
                _next = blockOffset;
                _allocated -= sizeofBlock;
            }
            else
            {
                Offset freeAwaitList = _atomicFreeAwaitList.load(std::memory_order_acquire);

                for(;;)
                {
                    if(_badOffset == freeAwaitList)
                    {
                        block->_freeAwaitListMember._next = _badOffset;
                        block->_freeAwaitListMember._last = blockOffset;
                        block->_freeAwaitListMember._size = sizeofBlock;
                    }
                    else
                    {
                        Block *old = Offset2Block(freeAwaitList);
                        block->_freeAwaitListMember._next = freeAwaitList;
                        block->_freeAwaitListMember._last = old->_freeAwaitListMember._last;
                        block->_freeAwaitListMember._size = old->_freeAwaitListMember._size+sizeofBlock;
                    }

                    if(_atomicFreeAwaitList.compare_exchange_strong(freeAwaitList, blockOffset, std::memory_order_release, std::memory_order_acquire))
                    {
                        break;
                    }
                }
            }
        }

        bool empty() const
        {
            return (_allocated << 1) ? false : true;
        }

    };


    template <uint32_t sizeofBlock, uint32_t blocksAmount>
    __thread std::thread::native_handle_type Pool_c<sizeofBlock, blocksAmount>::_threadId = std::thread::native_handle_type();

}
