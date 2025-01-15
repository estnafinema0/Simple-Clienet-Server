#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

#define BUFFER_LEN 256
#define QUEUE 4

const char *operation_names[] = {
    "Invalid",      
    "Addition",     
    "Subtraction",  
    "Multiplication",
    "Division",     
    "Pow", 
    "Exit"           
};

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
    
    listen(sockfd, QUEUE);
    newsockfd = accept(sockfd, (struct sockaddr *)&client_add, &cli_len);
    if (newsockfd < 0)
        error("Client accept");
    puts("Client connected.");

    int operand1, operand2, choice;
    double answer;

    while (1) {
        // Get operand 1
        bytes_rw = write(newsockfd, "Enter 1st operand: ", strlen("Enter 1st operand: "));
        if (bytes_rw < 0) error("Write from server side: ");
        bytes_rw = read(newsockfd, &operand1, sizeof(int));
        if (bytes_rw <= 0) error("Read from client side: ");
        printf("Client 1st operand: %d\n", operand1);

        // Get 2nd operand 
        bytes_rw = write(newsockfd, "Enter 2nd operand: ", strlen("Enter 2nd operand: "));
        if (bytes_rw < 0) error("Write from server side: ");
        bytes_rw = read(newsockfd, &operand2, sizeof(int));
        if (bytes_rw <= 0) error("Read from client side: ");
        printf("Client 2nd operand: %d\n", operand2);

        // Get operator
        char choices[] = "Select operator.\n1 - Addition\n2 - Subtraction\n3 - Multiplication\n4 - Division\n5 - Pow\n6 - Exit\nEnter your choice's number: ";
        bytes_rw = write(newsockfd, choices, strlen(choices));
        if (bytes_rw < 0) error("Write from server side: ");
        
        while(1) {
            bytes_rw = read(newsockfd, &choice, sizeof(int));
            if (bytes_rw <= 0)
            {
                error("Read from client side: ");
                goto QUIT;
            }
            if (choice < 1 || choice > 6) 
            {
                bytes_rw = write(newsockfd, "Invalid choice. Try again.", strlen("Invalid choice. Try again."));
                if (bytes_rw < 0) error("Write from server side: ");
                continue;
            }
            break;
        }

        printf("Client choice: %s\n", operation_names[choice]);
        if (choice == 6) {
            printf("Client requested exit.\n");
            break;
        }
        // Calculating result
        switch(choice)
        {
            case 1: 
                answer = operand1 + operand2; break;
            case 2: 
                answer = operand1 - operand2; break;
            case 3: 
                answer = operand1 * operand2; break;
            case 4: 
                if (operand2 == 0) {
                    write(newsockfd, "Error: division by zero!\n", strlen("Error: division by zero!\n"));
                    continue;
                }
                answer = (double)operand1 / (double)operand2; break;
            case 5: 
                answer = pow((double)operand1, (double)operand2); break;
            default: 
                answer = 0.0;
                break;
        }
        memset(buffer, 0, BUFFER_LEN);
        sprintf(buffer, "Answer: %.2lf\n\n--- New operation ---\n", answer);
        bytes_rw = write(newsockfd, buffer, strlen(buffer));
        if (bytes_rw < 0) error("Write from server side: ");
    }

QUIT:    
    printf("Closing connection...\n");
    close(newsockfd);
    close(sockfd);
    return 0;
}