#pragma once

#include "lut/io/sizeofImpl.hpp"
#include "lut/hiddenImpl.hpp"
#include "lut/async/future.hpp"

namespace lut { namespace io
{
    namespace impl
    {
        class Handle;
    }

    class Handle
        : private HiddenImpl<impl::Handle>
    {
        Handle(const Handle &from) = delete;
        Handle &operator=(const Handle &from) = delete;

    public:
        Handle();
        ~Handle();

    public:
        bool isActive();
        bool isClosing();
        async::Future<> close();
    };
}}
