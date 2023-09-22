// sender.c - UDP Sender
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define UDP_PORT 54321
#define MAX_STRING_LENGTH 100

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set up server_addr structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    inet_pton(AF_INET, "Receiver_IP_Address", &server_addr.sin_addr);

    printf("Enter strings to send (q to quit):\n");

    while (1) {
        char input[MAX_STRING_LENGTH];
        printf("> ");
        fgets(input, MAX_STRING_LENGTH, stdin);

        // Check if the user wants to quit
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }

        // Send the string to the receiver using UDP
        if (sendto(sockfd, input, strlen(input), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("Send error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    // Close the socket
    close(sockfd);
    printf("Sender program terminated.\n");
    return 0;
}

