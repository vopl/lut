#pragma once

#include "lut/io/stream.hpp"
#include <sys/socket.h>

namespace lut { namespace io
{
    namespace impl
    {
        class Tcp;
    }

    class Tcp
        : public Stream
        , private HiddenImpl<impl::Tcp>
    {
        Tcp(const Tcp &from) = delete;
        Tcp &operator=(const Tcp &from) = delete;

    public:
        Tcp();
        ~Tcp();

    public:

        std::error_code open(int sock);

        std::error_code nodelay(bool enable);
        std::error_code keepalive(bool enable, unsigned int delay);
        std::error_code simultaneous_accepts(bool enable);

        std::error_code bind(const sockaddr* addr, unsigned int flags);
        std::error_code getsockname(sockaddr* name, int* namelen);
        std::error_code getpeername(sockaddr* name, int* namelen);
        async::Future<std::error_code> connect(const sockaddr* addr);
    };
}}
