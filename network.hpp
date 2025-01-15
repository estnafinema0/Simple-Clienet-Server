#pragma once

namespace Net
{
    class Socket
    {
        int m_Sockfd;
    public:
        Socket(int domain, int type);
        ~Socket();
    };
}