#ifndef _ASYNC_SIZEOFIMPL_HPP_
#define _ASYNC_SIZEOFIMPL_HPP_

#include <cstddef>

namespace lut { namespace async
{
    template <class T>
    struct sizeofImpl
    {
    };

    namespace impl { class ThreadContainer; } 
    template <> struct sizeofImpl<impl::ThreadContainer> { static const std::size_t _value =  1;};

    namespace impl { class CoroContainer; } 
    template <> struct sizeofImpl<impl::CoroContainer> { static const std::size_t _value =  1;};

    namespace impl { class Event; } 
    template <> struct sizeofImpl<impl::Event> { static const std::size_t _value =  1;};

    namespace impl { class Mutex; } 
    template <> struct sizeofImpl<impl::Mutex> { static const std::size_t _value =  1;};

}}

#endif
