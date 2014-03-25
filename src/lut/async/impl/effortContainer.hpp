#ifndef _LUT_ASYNC_IMPL_EFFORTCONTAINER_HPP_
#define _LUT_ASYNC_IMPL_EFFORTCONTAINER_HPP_

namespace lut { namespace async { namespace impl
{
    template <class T>
    class EffortContainer
    {

    public:
        EffortContainer();
        ~EffortContainer();

        bool empty();
        void enqueue(T *v);
        T *dequeue();

    private:
        T *_first;
        T *_last;
    };



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    EffortContainer<T>::EffortContainer()
        : _first(nullptr)
        , _last(nullptr)
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    EffortContainer<T>::~EffortContainer()
    {
        assert(!_first);
        assert(!_last);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    bool EffortContainer<T>::empty()
    {
        return !_first;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    void EffortContainer<T>::enqueue(T *v)
    {
        v->_nextInList = nullptr;

        if(_last)
        {
            _last->_nextInList = v;
            _last = v;
        }
        else
        {
            _first = _last = v;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    T *EffortContainer<T>::dequeue()
    {
        if(_first)
        {
            T *v = _first;
            _first = _first->_nextInList;
            if(!_first)
            {
                _last = nullptr;
            }
            return v;
        }

        return nullptr;
    }
}}}

#endif

