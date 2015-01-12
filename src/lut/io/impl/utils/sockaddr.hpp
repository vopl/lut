#pragma once

#include "lut/io/endpoint.hpp"

#include <type_traits>
#include <netinet/in.h>

namespace lut { namespace io { namespace impl { namespace utils
{

    template <typename Address>
    using Sockaddr = typename std::conditional
    <
        std::is_same<Address, io::ip4::Address>::value,
        sockaddr_in,
        typename std::conditional
            <
                std::is_same<Address, io::ip6::Address>::value,
                sockaddr_in6,
                void
            >::type
    >::type;

    int fillSockaddr(sockaddr_in &inetAddress, const io::ip4::Address &address, std::uint16_t port);
    int fillSockaddr(sockaddr_in6 &inetAddress, const io::ip6::Address &address, std::uint16_t port);

}}}}
