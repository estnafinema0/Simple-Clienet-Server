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

    Socket::~Socket()
    {
        close(m_Sockfd);
    }
}