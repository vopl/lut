#ifndef _ASYNC_HIDDENIMPL_HPP_
#define _ASYNC_HIDDENIMPL_HPP_

#include "async/sizeofImpl.hpp"
#include <type_traits>

namespace async
{
    template <class T>
    class HiddenImpl
    {
    private:
        HiddenImpl(const HiddenImpl &other) = delete;
        HiddenImpl &operator=(const HiddenImpl &other) = delete;

    protected:

        template <typename... Arg>
        HiddenImpl(const Arg &... args)
        {
            static_assert(sizeof(T)<=sizeof(_data), "inconsistent sizeofImpl");
            new (&_data) T(args...);
        }

        ~HiddenImpl()
        {
            (&impl())->~T();
        }

        T &impl()
        {
            return *static_cast<T*>(static_cast<void*>(&_data));
        }

    private:
        typename std::aligned_storage<sizeofImpl<T>::_value>::type _data;
    };
}

#endif
