#pragma once

#include "lut/mm/utils/intrusiveDeque.hpp"

namespace lut { namespace mm { namespace utils
{

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    IntrusiveDeque<T>::IntrusiveDeque()
        : _first{}
        , _last{}
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    IntrusiveDeque<T>::IntrusiveDeque(T *element)
        : _first{element}
        , _last{element}
    {
        element->_prev = element->_next = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    IntrusiveDeque<T>::~IntrusiveDeque()
    {
        assert(!_first && !_last);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    bool IntrusiveDeque<T>::empty() const
    {
        assert(!!_first == !!_last);
        return !_first;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    T *IntrusiveDeque<T>::first() const
    {
        return _first;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    T *IntrusiveDeque<T>::last() const
    {
        return _last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    bool IntrusiveDeque<T>::contain(T *element) const
    {
        T *first = element;
        //T *last = element;

        while(first->_prev) first = first->_prev;
        //while(last->_next) last = last->_next;

        return _first == first ;//&& _last == last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void IntrusiveDeque<T>::push(T *element)
    {
        if(_last)
        {
            assert(_first);

            element->_prev = _last;
            element->_next = nullptr;

            assert(nullptr == _last->_next);
            _last->_next = element;
            _last = element;
        }
        else
        {
            assert(!_first);

            element->_prev = nullptr;
            element->_next = nullptr;

            _first = element;
            _last = element;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void IntrusiveDeque<T>::remove(T *element)
    {
        assert(contain(element));

        if(_first == element)
        {
            _first = element->_next;
        }
        if(_last == element)
        {
            _last = element->_prev;
        }

        if(element->_prev)
        {
            assert(element->_prev->_next == element);

            element->_prev->_next = element->_next;
        }
        if(element->_next)
        {
            assert(element->_next->_prev == element);

            element->_next->_prev = element->_prev;
        }

        element->_prev = nullptr;
        element->_next = nullptr;
    }
}}}
