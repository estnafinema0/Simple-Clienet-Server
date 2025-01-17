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
#include <thread>
#include <mutex>
#include <vector>

#define BUFFER_LEN 256

std::mutex mutex;

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
    std::vector<std::thread> threads;

    while (1)
    {
        Socket client = server.Accept();
        
        ++clientnum;
        threads.emplace_back(newClientthread, std::move(client), clientnum); // creating thread for new client
    }

    for(auto &thread: threads)
        thread.join();

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
        
        {
            std::lock_guard<std::mutex> guard(mutex);
            printf("Client, id - %d, pid - %d: %s", clientnum, getpid(), buffer);
            
            bzero(buffer, BUFFER_LEN - 1);
            printf("MEServer, id - %d, pid - %d: %s", clientnum, getpid(), buffer);
            fgets(buffer, BUFFER_LEN - 1, stdin);
        }

        bytes_rw = client.WriteTo(std::span(buffer, BUFFER_LEN - 1));
        if (bytes_rw < 0)
            error("Write from server side: ");
        
        char termmsg[] = "revoir";
        if (strncmp(termmsg, buffer, strlen(termmsg)) == 0)
            break;
    }
}