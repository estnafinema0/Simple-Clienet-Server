#include "network.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>

#define BUFFER_LEN 256


void newClientthread(Net::Socket client, int clientnum);

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

    int portno = atoi(argv[1]), clientnum = 0;

    Socket server(AF_INET, SOCK_STREAM);
    
    if (!server.Bind(InetSocketAddress(portno), REUSE_PORT))
        error("Binding failed");

    server.Listen(4);
    std::vector<pid_t> pids;
    while (1)
    {
        Socket client = server.Accept();
        
        pid_t pid;
        ++clientnum;
        if ((pid = fork()) == -1) error("Fork");
        else if (pid == 0) newClientthread(std::move(client), clientnum);
        
        pids.push_back(pid);
        for (auto pid: pids)
            waitpid(pid, nullptr, WNOHANG);
    }

    return 0;
}

void newClientthread(Net::Socket client, int clientnum)
{
    char buffer[BUFFER_LEN] = {};
    int bytes_rw;
    while(1)
    {
        bzero(buffer, BUFFER_LEN - 1);
        bytes_rw = client.ReadFromInto(std::span(buffer, BUFFER_LEN - 1));
        if (bytes_rw < 0)
            error("Read from client side: ");
        printf("Client, id - %d, pid - %d: %s", clientnum, getpid(), buffer);

        bzero(buffer, BUFFER_LEN - 1);
        printf("MEServer, id - %d, pid - %d: %s", clientnum, getpid(), buffer);
        fgets(buffer, BUFFER_LEN - 1, stdin);
        bytes_rw = client.WriteTo(std::span(buffer, BUFFER_LEN - 1));
        if (bytes_rw < 0)
            error("Write from server side: ");
        
        char termmsg[] = "revoir";
        if (strncmp(termmsg, buffer, strlen(termmsg)) == 0)
            break;
    }
    exit(EXIT_SUCCESS);
}