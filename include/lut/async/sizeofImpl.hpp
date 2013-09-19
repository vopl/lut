#ifndef _ASYNC_SIZEOFIMPL_HPP_
#define _ASYNC_SIZEOFIMPL_HPP_

#include <cstddef>

namespace lut { namespace async
{
    template <class T>
    struct sizeofImpl
    {
    };

    namespace impl { class Scheduler; } 
    template <> struct sizeofImpl<impl::Scheduler> { static const std::size_t _value =  1;};

    namespace impl { class Event; } 
    template <> struct sizeofImpl<impl::Event> { static const std::size_t _value =  1;};

    namespace impl { class Mutex; } 
    template <> struct sizeofImpl<impl::Mutex> { static const std::size_t _value =  1;};

}}

#endif
