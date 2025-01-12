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
    if (argc != 3)
    {
        fprintf(stderr, "Usage: <program file> <hostname> <port>\n"); //hostname is the server ip address
        exit(EXIT_FAILURE);
    }

    int bytes_rw, portno = atoi(argv[2]);
    int sockfd;
    char buffer[BUFFER_LEN];
    
    struct sockaddr_in serv_addr;

    struct hostent *server;
    server = gethostbyname(argv[1]);
    if (server == NULL) 
        fprintf(stderr, "Wrong hostname name provided or the host is off");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Server socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");
    
    while(1) 
    {
        bzero(buffer, BUFFER_LEN - 1);
        printf("MEClient: %s", buffer);
        fgets(buffer, BUFFER_LEN - 1, stdin);
        bytes_rw = write(sockfd, buffer, BUFFER_LEN - 1);
        if (bytes_rw < 0)
            error("Write from client side: ");

        bzero(buffer, BUFFER_LEN - 1);
        bytes_rw = read(sockfd, buffer, BUFFER_LEN - 1);
        if (bytes_rw < 0)
            error("Read from server side: ");
        printf("Server: %s", buffer);

        char termmsg[] = "revoir";
        if (strncmp(termmsg, buffer, strlen(termmsg)) == 0)
            break;
    }

    close(sockfd);

    return 0;
}