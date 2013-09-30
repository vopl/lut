#include "lut/async/stable.hpp"
#include "lut/async/impl/coroContainer.hpp"
#include "lut/async/impl/coro.hpp"

namespace lut { namespace async { namespace impl
{

    CoroContainer::CoroContainer()
        : _lock(0)
        , _size(0)
        , _head(nullptr)
        , _tail(nullptr)
    {
    }

    CoroContainer::~CoroContainer()
    {
        assert(emptyRelaxed());
    }

    void CoroContainer::enqueue(Coro *coro)
    {
        coro->_nextForContainer = nullptr;

        lock();

        if(!_tail)
        {
            assert(!_head);
            _tail = _head = coro;

            assert(!_size.load(std::memory_order_relaxed));
            _size.store(1, std::memory_order_relaxed);
        }
        else
        {
            assert(_head);
            _tail->_nextForContainer = coro;
            _tail = coro;

            assert(_size.load(std::memory_order_relaxed));
            _size.fetch_add(1, std::memory_order_relaxed);
        }

        unlock();
    }

    Coro *CoroContainer::dequeue()
    {
        lock();

        Coro *coro;
        if(!_head)
        {
            assert(!_tail);
            coro = nullptr;
        }
        else
        {
            assert(_tail);
            coro = _head;
            _head = _head->_nextForContainer;
            if(!_head)
            {
                assert(coro == _tail);
                _tail = nullptr;

                assert(1 == _size.load(std::memory_order_relaxed));
                _size.store(0, std::memory_order_relaxed);
            }
            else
            {
                assert(1 < _size.load(std::memory_order_relaxed));
                _size.fetch_sub(1, std::memory_order_relaxed);
            }
        }

        unlock();

        return coro;
    }

    size_t CoroContainer::sizeRelaxed() const
    {
        return _size.load(std::memory_order_relaxed);
    }

    bool CoroContainer::emptyRelaxed() const
    {
        return sizeRelaxed() ? false : true;
    }

    void CoroContainer::gripFrom(CoroContainer &from)
    {
        from.lock();

        if(from._head)
        {
            assert(from._tail);
            if(_head)
            {
                assert(_tail);

                _tail->_nextForContainer = from._head;
                _tail = from._tail;

                from._head = from._tail = nullptr;
                size_t fromSize = from._size.exchange(0, std::memory_order_relaxed);
                _size.fetch_add(fromSize, std::memory_order_relaxed);
            }
            else
            {
                assert(!_tail);

                _head = from._head;
                _tail = from._tail;

                from._head = from._tail = nullptr;
                size_t fromSize = from._size.exchange(0, std::memory_order_relaxed);
                _size.store(fromSize, std::memory_order_relaxed);
            }
        }
        else
        {
            assert(!from._tail);
        }
        from.unlock();
    }

    void CoroContainer::lock()
    {
        while(_lock.exchange(true, std::memory_order_acquire));
    }

    void CoroContainer::unlock()
    {
        _lock.store(false, std::memory_order_release);
    }

}}}


