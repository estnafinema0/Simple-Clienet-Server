#include "network.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

namespace
{
    void error(const char *msg)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}


namespace Net
{
    Socket::Socket(int domain, int type)
    {
        m_Sockfd = socket(domain, type, 0);
        if (m_Sockfd < 0)
            error("Failed to create socket");
    }

    Socket::Socket(int sockfd)
        : m_Sockfd(sockfd)
    {}

    Socket::~Socket()
    {
        close(m_Sockfd);
    }

    Socket& Socket::operator=(Socket&& sock)
    {
        m_Sockfd = sock.m_Sockfd;
        sock.m_Sockfd = -1;
        return *this;
    }
    
    void Socket::Bind(const InetSocketAddress& addr)
    {
        bind(m_Sockfd, addr.AsSockaddr(), sizeof(sockaddr_in));
    }

    void Socket::Bind(const InetSocketAddress& addr, const SocketOptions& opts)
    {
        if ((opts & REUSE_PORT).any())
        {
            int option = 1;
            setsockopt(m_Sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        }
        static_assert(TOTAL_SOCKET_OPTIONS == 1);
        
        Bind(addr);
    }

    void Socket::Listen(int queue_length)
    {
        listen(m_Sockfd, queue_length);
    }

    Socket Socket::AcceptInet()
    {
        InetSocketAddress addr{};
        socklen_t length = sizeof(sockaddr_in);
        return Socket(accept(m_Sockfd, addr.AsSockaddr(), &length));
    }

    InetSocketAddress::InetSocketAddress(int port)
    {
        m_Addr.sin_port = htons(port);
        m_Addr.sin_family = AF_INET;
        m_Addr.sin_addr.s_addr = INADDR_ANY;
    }

    const sockaddr* InetSocketAddress::AsSockaddr() const&
    {
        return reinterpret_cast<const sockaddr*>(&m_Addr);
    }
    
    sockaddr* InetSocketAddress::AsSockaddr() &
    {
        return reinterpret_cast<sockaddr*>(&m_Addr);
    }
}
