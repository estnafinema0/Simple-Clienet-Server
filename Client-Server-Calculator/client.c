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
    
    printf("Connected to server: %s:%d\n", argv[1], portno);

    int operand1, operand2, choice;

    while(1){
        bzero(buffer, BUFFER_LEN); // Read server message
        bytes_rw = read(sockfd, buffer, BUFFER_LEN);
        if (bytes_rw <= 0) error("Read from server side: ");
        printf("SERVER. %s", buffer);

        // Send operand 1
        while (1) {
            if (scanf("%d", &operand1) == 1) {
                break;
            } else {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n');
            }
        }
        bytes_rw = write(sockfd, &operand1, sizeof(int));
        if (bytes_rw < 0) error("Write from client side: ");

        bzero(buffer, BUFFER_LEN); // Read server message
        bytes_rw = read(sockfd, buffer, BUFFER_LEN);
        if (bytes_rw <= 0) error("Read from server side: ");
        printf("SERVER. %s", buffer);
        
        // Send operand 2
        while (1) {
            if (scanf("%d", &operand2) == 1) {
                break;
            } else {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n');
            }
        }
        bytes_rw = write(sockfd, &operand2, sizeof(int));
        if (bytes_rw < 0) error("Write from client side: ");

        // Make choice
        bzero(buffer, BUFFER_LEN); // Read server message
        bytes_rw = read(sockfd, buffer, BUFFER_LEN);
        if (bytes_rw <= 0) error("Read from server side: ");
        printf("SERVER. %s\n", buffer);
        
        // Send operator
        while (1) {
            if (scanf("%d", &choice) == 1) {
                bytes_rw = write(sockfd, &choice, sizeof(int));
                if (bytes_rw < 0) error("Write from client side: ");

                if (choice < 1 || choice > 6) {
                    memset(buffer, 0, BUFFER_LEN);
                    bytes_rw = read(sockfd, buffer, BUFFER_LEN - 1);
                    if (bytes_rw <= 0) error("Read from server side: ");
                    printf("SERVER: %s", buffer);
                    continue;
                }
                break;
            } else {
                printf("Invalid input. Please enter a number (1..6): ");
                while (getchar() != '\n');
            }
        }

        if (choice == 6) 
        {   
            puts("You have selected to quit. The program is terminating...");
            break;
        }

        // Get answer
        bzero(buffer, BUFFER_LEN); // Read server message
        bytes_rw = read(sockfd, buffer, BUFFER_LEN);
        if (bytes_rw <= 0) error("Read from server side: ");
        printf("SERVER. %s\n", buffer);
    }

    close(sockfd);

    return 0;
}