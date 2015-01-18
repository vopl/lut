#pragma once

#include "lut/hiddenImpl/sizeProvider.hpp"
#include <utility>

namespace lut { namespace hiddenImpl
{
    class Accessor;

    template <class T>
    class Single
    {
    public:
        using Impl = T;

    protected:
        friend class hiddenImpl::Accessor;

        template <typename... Arg>
        Single(const Arg &... args);

        Single(const Single &other);
        Single(Single &&other);

        Single(const T &other);
        Single(T &&other);

        ~Single();

        Single &operator=(const Single &other);
        Single &operator=(Single &&other);

        Single &operator=(const T &other);
        Single &operator=(T &&other);

        T *pimpl();
        T &impl();

        const T *pimpl() const;
        const T &impl() const;

    private:
        char _data[hiddenImpl::sizeProvider<T>::_value];
    };




    namespace
    {
        template <typename T, std::size_t s1=sizeof(T), std::size_t s2 = sizeof(Single<T>)>
        void sizeChecker()
        {
            static_assert(sizeof(T)==sizeof(Single<T>), "inconsistent sizeProvider");
        }
    }




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    template <typename... Arg>
    Single<T>::Single(const Arg &... args)
    {
        sizeChecker<T>();
        new (&_data) T(args...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T>::Single(const Single &other)
    {
        sizeChecker<T>();
        new (&_data) T(other.impl());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T>::Single(Single &&other)
    {
        sizeChecker<T>();
        new (&_data) T(std::move(other.impl()));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T>::Single(const T &other)
    {
        sizeChecker<T>();
        new (&_data) T(other);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T>::Single(T &&other)
    {
        sizeChecker<T>();
        new (&_data) T(std::move(other));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T>::~Single()
    {
        pimpl()->~T();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T> &Single<T>::operator=(const Single &other)
    {
        impl() = other.impl();
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T> &Single<T>::operator=(Single &&other)
    {
        impl() = std::forward<T>(other.impl());
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T> &Single<T>::operator=(const T &other)
    {
        impl() = other;
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    Single<T> &Single<T>::operator=(T &&other)
    {
        impl() = std::forward<T>(other);
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    T *Single<T>::pimpl()
    {
        return reinterpret_cast<T*>(&_data);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    T &Single<T>::impl()
    {
        return *pimpl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    const T *Single<T>::pimpl() const
    {
        return reinterpret_cast<const T*>(&_data);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    const T &Single<T>::impl() const
    {
        return *pimpl();
    }
}}
