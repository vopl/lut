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
        T *_head;
    };



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    EffortContainer<T>::EffortContainer()
        : _head(nullptr)
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    EffortContainer<T>::~EffortContainer()
    {
        assert(!_head);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    bool EffortContainer<T>::empty()
    {
        return !_head;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    void EffortContainer<T>::enqueue(T *v)
    {
        v->_nextInList = _head;
        _head = v;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    T *EffortContainer<T>::dequeue()
    {
        if(_head)
        {
            T *v = _head;
            _head = _head->_nextInList;
            return v;
        }

        return nullptr;
    }
}}}

#endif

