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

            std::atomic<Element*> _next;
        };

    public:
        IntrusiveQueue();
        ~IntrusiveQueue();

        void enqueue(Element *element);
        Element *dequeue();

    private:
        std::atomic_bool        _headLock;
        std::atomic<Element*>   _head;

        std::atomic_bool        _tailLock;
        std::atomic<Element*>   _tail;
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
        while(_tailLock.exchange(true));

        Element *tail = _tail.load();

        if(tail)
        {
            element->_next.store(nullptr);
            _tail.store(element);
            tail->_next.store(element);

            _tailLock.store(false);
            return;
        }

        while(_headLock.exchange(true));
        //Element *head = _head.load();
        //assert(!head);

        //if(!head)
        {
            element->_next.store(nullptr);
            _tail.store(element);
            _head.store(element);

            _tailLock.store(false);
            _headLock.store(false);
            return;
        }

        assert(!"never here");
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    Element *IntrusiveQueue<Element>::dequeue()
    {
        while(_headLock.exchange(true));

        Element *head = _head.load();
        if(!head)
        {
            _headLock.store(false);
            return nullptr;
        }

        Element *next = head->_next.load();

        if(next)
        {
            _head.store(next);
            _headLock.store(false);
            /*unwanted*/ head->_next.store(nullptr);
            return head;
        }

        bool tailLockWas = false;
        if(_tailLock.compare_exchange_strong(tailLockWas, true))
        {
            Element *tail = _tail.load();

            if(head == tail)
            {
                _head.store(nullptr);
                _tail.store(nullptr);
                _headLock.store(false);
                _tailLock.store(false);
                return head;
            }

            _tailLock.store(false);
        }

        do
        {
            next = head->_next.load();
        }
        while(!next);

        _head.store(next);
        _headLock.store(false);
        /*unwanted*/ head->_next.store(nullptr);
        return head;
    }

}}}

#endif
