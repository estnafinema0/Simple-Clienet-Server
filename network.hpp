#pragma once

#include <bitset>
#include <netinet/in.h>

namespace Net
{
    constexpr int TOTAL_SOCKET_OPTIONS = 1;
    using SocketOptions = std::bitset<TOTAL_SOCKET_OPTIONS>;
    constexpr SocketOptions REUSE_PORT(0x1);

    class InetSocketAddress
    {
        sockaddr_in m_Addr;
    public:
        InetSocketAddress(int port);
        
        const sockaddr* AsSockaddr() const&;
    };

    class Socket
    {
        int m_Sockfd;
    public:
        Socket(int domain, int type);
        ~Socket();

        void Bind(const InetSocketAddress& addr);
        void Bind(const InetSocketAddress& addr, const SocketOptions& opts);
    };
}