#ifndef _LUT_ASYNC_IMPL_QUEUE_HPP_
#define _LUT_ASYNC_IMPL_QUEUE_HPP_

#include <atomic>

namespace lut { namespace async { namespace impl
{

    template <typename Element>
    class Queue
    {
    public:
        struct ElementBase
        {
            ElementBase();

            Element *_next;
        };

    public:
        Queue();
        ~Queue();

        void enqueue(Element *element);
        Element *dequeue();

    private:
        std::atomic<Element*> _head;
        std::atomic<Element*> _tail;

        Element * const _busyValue = (Element *)1;

    };

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    Queue<Element>::ElementBase::ElementBase()
        : _next()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    Queue<Element>::Queue()
        : _head()
        , _tail()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    Queue<Element>::~Queue()
    {
        assert(!_head);
        assert(!_tail);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    void Queue<Element>::enqueue(Element *element)
    {
        assert(0);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename Element>
    Element *Queue<Element>::dequeue()
    {
        Element *element = _head.load();

        for(;;)
        {
            if(_busyValue < element)
            {
                if(!_head.compare_exchange_strong(element, _busyValue))
                {
                    element = _head.load();
                    continue;
                }

                _head.store(element->_next);
                element->_next = nullptr;
                break;
            }

            if(!element)
            {
                break;
            }

            assert(_busyValue == element);
            _head.load();
        }

        return element;
    }

}}}

#endif
