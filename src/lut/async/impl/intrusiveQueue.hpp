#ifndef _LUT_ASYNC_IMPL_INTRUSIVEQUEUE_HPP_
#define _LUT_ASYNC_IMPL_INTRUSIVEQUEUE_HPP_

#include <atomic>

namespace lut { namespace async { namespace impl
{

    template <typename Element>
    class IntrusiveQueue
    {
    public:
        struct ElementBase
        {
            ElementBase();

            char pad1[256];
            std::atomic<Element*> _next;
            char pad2[256];
        };

    public:
        IntrusiveQueue();
        ~IntrusiveQueue();

        void enqueue(Element *element);
        Element *dequeue();

    private:
        char pad1[256];
        std::atomic_bool        _headLock;
        std::atomic<Element*>   _head;

        char pad3[256];

        std::atomic_bool        _tailLock;
        std::atomic<Element*>   _tail;
        char pad5[256];
    };

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    IntrusiveQueue<Element>::ElementBase::ElementBase()
        : _next()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    IntrusiveQueue<Element>::IntrusiveQueue()
        : _headLock()
        , _head()
        , _tailLock()
        , _tail()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    IntrusiveQueue<Element>::~IntrusiveQueue()
    {
        assert(!_head);
        assert(!_tail);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    void IntrusiveQueue<Element>::enqueue(Element *element)
    {
        element->_next.store(nullptr, std::memory_order_relaxed);

        while(_tailLock.exchange(true, std::memory_order_acquire));

        Element *tail = _tail.load(std::memory_order_relaxed);

        if(tail)
        {
            tail->_next.store(element, std::memory_order_relaxed);
            _tail.store(element, std::memory_order_relaxed);

            _tailLock.store(false, std::memory_order_release);
            return;
        }

        while(_headLock.exchange(true, std::memory_order_acquire));
        //Element *head = _head.load(std::memory_order_relaxed);
        //assert(!head);

        //if(!head)
        {
            _tail.store(element, std::memory_order_relaxed);
            _head.store(element, std::memory_order_relaxed);

            _tailLock.store(false, std::memory_order_release);
            _headLock.store(false, std::memory_order_release);
            return;
        }

        assert(!"never here");
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    Element *IntrusiveQueue<Element>::dequeue()
    {
        while(_headLock.exchange(true, std::memory_order_acquire));

        Element *head = _head.load(std::memory_order_relaxed);
        if(!head)
        {
            _headLock.store(false, std::memory_order_release);
            return nullptr;
        }

        Element *next = head->_next.load(std::memory_order_relaxed);

        if(next)
        {
            _head.store(next, std::memory_order_relaxed);
            _headLock.store(false, std::memory_order_release);
            // /*unwanted*/ head->_next.store(nullptr, std::memory_order_relaxed);
            return head;
        }

        if(!_tailLock.exchange(true, std::memory_order_acquire))
        {
            Element *tail = _tail.load(std::memory_order_relaxed);

            if(head == tail)
            {
                _head.store(nullptr, std::memory_order_relaxed);
                _tail.store(nullptr, std::memory_order_relaxed);
                _headLock.store(false, std::memory_order_release);
                _tailLock.store(false, std::memory_order_release);
                return head;
            }

            _tailLock.store(false, std::memory_order_release);
        }

        do
        {
            next = head->_next.load(std::memory_order_relaxed);
        }
        while(!next);

        _head.store(next, std::memory_order_relaxed);
        _headLock.store(false, std::memory_order_release);
        // /*unwanted*/ head->_next.store(nullptr);
        return head;
    }

}}}

#endif
