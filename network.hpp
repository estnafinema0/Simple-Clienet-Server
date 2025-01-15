#pragma once

#include <span>
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
        InetSocketAddress(int port, std::span<char> address);
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

        bool Bind(const InetSocketAddress& addr);
        bool Bind(const InetSocketAddress& addr, const SocketOptions& opts);
        void Listen(int queue_length);
        Socket AcceptInet();
        bool Connect(const InetSocketAddress& addr);

        int WriteTo(std::span<char> buf);
        int ReadFromInto(std::span<char> buf);
    };
}
