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
        explicit InetSocketAddress() = default;
        
        const sockaddr* AsSockaddr() const&;
        sockaddr* AsSockaddr() &;
    };

    class Socket
    {
        int m_Sockfd;
    public:
        Socket(int domain, int type);
        explicit Socket(int sockfd);
        ~Socket();

        Socket& operator=(const Socket&) = delete;
        Socket& operator=(Socket&& sock);

        void Bind(const InetSocketAddress& addr);
        void Bind(const InetSocketAddress& addr, const SocketOptions& opts);
        void Listen(int queue_length);
        Socket AcceptInet();
    };
}
