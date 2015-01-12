#include "lut/stable.hpp"
#include "lut/io/impl/utils/sockaddr.hpp"

//#include <sys/socket.h>
//#include <sys/types.h>

namespace lut { namespace io { namespace impl { namespace utils
{

    int fillSockaddr(sockaddr_in &inetAddress, const io::ip4::Address &address, std::uint16_t port)
    {
        inetAddress.sin_family = AF_INET;
        inetAddress.sin_port = htons(port);
        inetAddress.sin_addr.s_addr = htonl(address._value._uint32);

        return AF_INET;
    }

    int fillSockaddr(sockaddr_in6 &inetAddress, const io::ip6::Address &address, std::uint16_t port)
    {
        inetAddress.sin6_family = AF_INET6;
        inetAddress.sin6_port = htons(port);
        memcpy(inetAddress.sin6_addr.__in6_u.__u6_addr8, address._value, sizeof(address._value));

        return AF_INET6;
    }

}}}}
