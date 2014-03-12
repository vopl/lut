#ifndef _LUT_ASYNC_IMPL_WORKER_EFFORTCONTAINER_HPP_
#define _LUT_ASYNC_IMPL_WORKER_EFFORTCONTAINER_HPP_

#include <boost/circular_buffer.hpp>

namespace lut { namespace async { namespace impl { namespace worker
{
    template <class T>
    class EffortContainer
        : private boost::circular_buffer_space_optimized<T>
    {
        using Base = boost::circular_buffer_space_optimized<T>;

    public:
        EffortContainer();
        ~EffortContainer();

        bool empty();
        void enqueue(T v);
        T dequeue();

        void moveTo(EffortContainer &target);
    };



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    EffortContainer<T>::EffortContainer()
        : Base(typename Base::capacity_type((std::size_t)-1, 128))
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    EffortContainer<T>::~EffortContainer()
    {

    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    bool EffortContainer<T>::empty()
    {
        return Base::empty();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    void EffortContainer<T>::enqueue(T v)
    {
        Base::push_back(v);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    T EffortContainer<T>::dequeue()
    {
        if(Base::empty())
        {
            return T();
        }

        T v = Base::front();
        Base::pop_front();

        return v;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    void EffortContainer<T>::moveTo(EffortContainer &target)
    {
        target.insert(target.end(), Base::begin(), Base::end());
        Base::clear();
    }

}}}}

#endif

