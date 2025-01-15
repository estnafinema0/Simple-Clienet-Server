#include "network.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
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
        : m_Domain(domain)
    {
        m_Sockfd = socket(m_Domain, type, 0);
        if (m_Sockfd < 0)
            error("Failed to create socket");
    }

    Socket::Socket(Socket&& sock)
        : m_Sockfd(sock.m_Sockfd), m_Domain(sock.m_Domain)
    {
        sock.m_Sockfd = -1;
    }

    Socket::~Socket()
    {
        close(m_Sockfd);
    }
    
    Socket Socket::FromExisting(int sockfd, int domain)
    {
        Socket sock{};
        sock.m_Sockfd = sockfd;
        sock.m_Domain = domain;
        
        return sock;
    }

    Socket& Socket::operator=(Socket&& sock)
    {
        m_Sockfd = sock.m_Sockfd;
        sock.m_Sockfd = -1;
        m_Domain = sock.m_Domain;
        return *this;
    }
    
    bool Socket::Bind(const InetSocketAddress& addr)
    {
        return bind(m_Sockfd, addr.AsSockaddr(), sizeof(sockaddr_in)) == 0;
    }

    bool Socket::Bind(const InetSocketAddress& addr, const SocketOptions& opts)
    {
        if ((opts & REUSE_PORT).any())
        {
            int option = 1;
            setsockopt(m_Sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        }
        static_assert(TOTAL_SOCKET_OPTIONS == 1);
        
        return Bind(addr);
    }

    void Socket::Listen(int queue_length)
    {
        listen(m_Sockfd, queue_length);
    }
    
    Socket Socket::Accept()
    {
        if (m_Domain == AF_INET) return AcceptInet();
        assert(0 && "Not implemented");
    }

    Socket Socket::AcceptInet()
    {
        InetSocketAddress addr{};
        socklen_t length = sizeof(sockaddr_in);
        return Socket::FromExisting(accept(m_Sockfd, addr.AsSockaddr(), &length), m_Domain);
    }

    bool Socket::Connect(const InetSocketAddress& addr)
    {
        return connect(m_Sockfd, addr.AsSockaddr(), sizeof(sockaddr_in)) == 0;
    }

    int Socket::WriteTo(std::span<char> buf)
    {
        return write(m_Sockfd, buf.data(), buf.size_bytes());
    }

    int Socket::ReadFromInto(std::span<char> buf)
    {
        return read(m_Sockfd, buf.data(), buf.size_bytes());
    }

    InetSocketAddress::InetSocketAddress(int port)
    {
        m_Addr.sin_port = htons(port);
        m_Addr.sin_family = AF_INET;
        m_Addr.sin_addr.s_addr = INADDR_ANY;
    }

    InetSocketAddress::InetSocketAddress(int port, std::span<char> address)
    {
        m_Addr.sin_port = htons(port);
        m_Addr.sin_family = AF_INET;
        assert(address.size_bytes() <= sizeof(m_Addr.sin_addr.s_addr));
        std::memcpy(&m_Addr.sin_addr.s_addr, address.data(), address.size_bytes());
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
