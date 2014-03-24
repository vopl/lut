#ifndef _LUT_ASYNC_HIDDENIMPL_HPP_
#define _LUT_ASYNC_HIDDENIMPL_HPP_

#if GENERATE_SIZEOFIMPL
namespace lut { namespace async
{
    template<class T> struct sizeofImpl
    {
        static const std::size_t _value = 1;
    };
}}
#else
#   include "lut/async/sizeofImpl.hpp"
#endif

namespace lut { namespace async
{
    template <class T>
    class HiddenImpl
    {
    private:
        HiddenImpl(const HiddenImpl &other) = delete;
        HiddenImpl &operator=(const HiddenImpl &other) = delete;

    protected:

        template <typename... Arg>
        HiddenImpl(const Arg &... args);
        ~HiddenImpl();

        T *pimpl();
        T &impl();

        const T *pimpl() const;
        const T &impl() const;

    private:
        char _data[sizeofImpl<T>::_value];
    };







    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    template <typename... Arg>
    HiddenImpl<T>::HiddenImpl(const Arg &... args)
    {
        static_assert(sizeof(T)==sizeof(HiddenImpl), "inconsistent sizeofImpl");
        new (&_data) T(args...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T>
    HiddenImpl<T>::~HiddenImpl()
    {
        pimpl()->~T();
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
}}

#endif
