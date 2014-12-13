#pragma once

#include "lut/hiddenImpl/sizeofImpl.hpp"
#include <utility>

namespace lut
{
    namespace hiddenImpl
    {
        class Accessor;
    }

    template <class T>
    class HiddenImpl
    {
    protected:
        friend class hiddenImpl::Accessor;

        template <typename... Arg>
        HiddenImpl(const Arg &... args);

        HiddenImpl(const HiddenImpl &other);
        HiddenImpl(HiddenImpl &&other);

        ~HiddenImpl();

        HiddenImpl &operator=(const HiddenImpl &other);
        HiddenImpl &operator=(HiddenImpl &&other);


        T *pimpl();
        T &impl();

        const T *pimpl() const;
        const T &impl() const;

    private:
        char _data[hiddenImpl::sizeofImpl<T>::_value];
    };




    namespace
    {
        template <typename T, std::size_t s1=sizeof(T), std::size_t s2 = sizeof(HiddenImpl<T>)>
        void sizeChecker()
        {
            static_assert(sizeof(T)==sizeof(HiddenImpl<T>), "inconsistent sizeofImpl");
        }
    }




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    template <typename... Arg>
    HiddenImpl<T>::HiddenImpl(const Arg &... args)
    {
        sizeChecker<T>();
        new (&_data) T(args...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    HiddenImpl<T>::HiddenImpl(const HiddenImpl &other)
    {
        sizeChecker<T>();
        new (&_data) T(other);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    HiddenImpl<T>::HiddenImpl(HiddenImpl &&other)
    {
        sizeChecker<T>();
        new (&_data) T(std::move(other.impl()));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    HiddenImpl<T>::~HiddenImpl()
    {
        pimpl()->~T();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    HiddenImpl<T> &HiddenImpl<T>::operator=(const HiddenImpl &other)
    {
        impl() = other.impl();
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    HiddenImpl<T> &HiddenImpl<T>::operator=(HiddenImpl &&other)
    {
        impl() = std::forward<T>(other.impl());
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    T *HiddenImpl<T>::pimpl()
    {
        return reinterpret_cast<T*>(&_data);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    T &HiddenImpl<T>::impl()
    {
        return *pimpl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    const T *HiddenImpl<T>::pimpl() const
    {
        return reinterpret_cast<const T*>(&_data);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    const T &HiddenImpl<T>::impl() const
    {
        return *pimpl();
    }
}
