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
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./server <port>\n");
        exit(EXIT_FAILURE);
    }

    int bytes_rw, portno = atoi(argv[1]);
    char buffer[BUFFER_LEN] = {};

    Socket server(AF_INET, SOCK_STREAM);
    
    if (!server.Bind(InetSocketAddress(portno), REUSE_PORT))
        error("Binding failed");

    server.Listen(4);
    Socket client = server.Accept();

    while (1)
    {
        bzero(buffer, BUFFER_LEN - 1);
        bytes_rw = client.ReadFromInto(std::span(buffer, BUFFER_LEN - 1));
        if (bytes_rw < 0)
            error("Read from client side: ");
        printf("Client: %s", buffer);

        bzero(buffer, BUFFER_LEN - 1);
        printf("MEServer: %s", buffer);
        fgets(buffer, BUFFER_LEN - 1, stdin);
        bytes_rw = client.WriteTo(std::span(buffer, BUFFER_LEN - 1));
        if (bytes_rw < 0)
            error("Write from server side: ");
        
        char termmsg[] = "revoir";
        if (strncmp(termmsg, buffer, strlen(termmsg)) == 0)
            break;
    }

    return 0;
}
