#include "network.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_LEN 256

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    using namespace Net;
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./client <hostname> <port>\n\n\t<hostname> is the server ip address\n");
        exit(EXIT_FAILURE);
    }

    int bytes_rw, portno = atoi(argv[2]);
    char buffer[BUFFER_LEN] = {};
    
    struct hostent *server;
    server = gethostbyname(argv[1]);
    if (server == NULL)
        fprintf(stderr, "Wrong hostname name provided or the host is off");

    Socket socket(AF_INET, SOCK_STREAM);
    if (!socket.Connect(InetSocketAddress(portno, std::span(server->h_addr_list[0], server->h_length))))
        error("Connection failed");
    
    while(1) 
    {
        bzero(buffer, BUFFER_LEN - 1);
        printf("MEClient: %s", buffer);
        fgets(buffer, BUFFER_LEN - 1, stdin);
        bytes_rw = socket.WriteTo(std::span(buffer, BUFFER_LEN - 1));
        if (bytes_rw < 0)
            error("Write from client side: ");

        bzero(buffer, BUFFER_LEN - 1);
        bytes_rw = socket.ReadFromInto(std::span(buffer, BUFFER_LEN - 1));
        if (bytes_rw < 0)
            error("Read from server side: ");
        printf("Server: %s", buffer);

        char termmsg[] = "revoir";
        if (strncmp(termmsg, buffer, strlen(termmsg)) == 0)
            break;
    }

    return 0;
}
