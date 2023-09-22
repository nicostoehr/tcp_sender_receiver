// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_STRING_LENGTH 100

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char input[MAX_STRING_LENGTH];

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set up server_addr structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to the receiver
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to receiver. Type 'q' to quit.\n");

    while (1) {
        printf("Enter a string to send (q to quit): ");
        fgets(input, MAX_STRING_LENGTH, stdin);

        // Check if the user wants to quit
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }

        // Send the string to the receiver
        if (send(sockfd, input, strlen(input), 0) == -1) {
            perror("Send error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    // Close the connection and socket
    close(sockfd);
    printf("Connection closed. Sender program terminated.\n");
    return 0;
}

