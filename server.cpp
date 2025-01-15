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
    if (argc != 2)
    {
        fprintf(stderr, "Usage: <program file> <port>\n");
        exit(EXIT_FAILURE);
    }

    int sockfd, newsockfd;
    int bytes_rw, portno = atoi(argv[1]);
    char buffer[BUFFER_LEN];

    struct sockaddr_in serv_add, client_add;
    socklen_t serv_len = sizeof(serv_add), cli_len = sizeof(client_add);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Server socket");

    int option = 1; //To reuse the same port 
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    bzero((char *)&serv_add, serv_len);
    serv_add.sin_port = htons(portno);
    serv_add.sin_family = AF_INET;
    serv_add.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv_add, serv_len) < 0)
        error("Binding failed");
    
    listen(sockfd, 4);
    newsockfd = accept(sockfd, (struct sockaddr *)&client_add, &cli_len);
    if (newsockfd < 0)
        error("CLient accept");

    while (1)
    {
        bzero(buffer, BUFFER_LEN - 1);
        bytes_rw = read(newsockfd, buffer, BUFFER_LEN - 1);
        if (bytes_rw < 0)
            error("Read from client side: ");
        printf("Client: %s", buffer);

        bzero(buffer, BUFFER_LEN - 1);
        printf("MEServer: %s", buffer);
        fgets(buffer, BUFFER_LEN - 1, stdin);
        bytes_rw = write(newsockfd, buffer, BUFFER_LEN - 1);
        if (bytes_rw < 0)
            error("Write from server side: ");
        
        char termmsg[] = "revoir";
        if (strncmp(termmsg, buffer, strlen(termmsg)) == 0)
            break;
    }

    close(sockfd);
    close(newsockfd);
    return 0;
}
