// receiver.c - Receiver with support for both TCP and UDP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TCP_PORT 12345
#define UDP_PORT 54321
#define MAX_STRING_LENGTH 100

int main() {
    int tcp_sockfd, udp_sockfd;
    struct sockaddr_in tcp_server_addr, udp_server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char received_string[MAX_STRING_LENGTH];

    // Create a TCP socket
    if ((tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("TCP Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set up TCP server_addr structure
    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_port = htons(TCP_PORT);
    tcp_server_addr.sin_addr.s_addr = INADDR_ANY;

    // Create a UDP socket
    if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("UDP Socket creation error");
        close(tcp_sockfd);
        exit(EXIT_FAILURE);
    }

    // Set up UDP server_addr structure
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_PORT);
    udp_server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind both sockets to their respective ports
    if (bind(tcp_sockfd, (struct sockaddr*)&tcp_server_addr, sizeof(tcp_server_addr)) == -1) {
        perror("TCP Binding error");
        close(tcp_sockfd);
        close(udp_sockfd);
        exit(EXIT_FAILURE);
    }

    if (bind(udp_sockfd, (struct sockaddr*)&udp_server_addr, sizeof(udp_server_addr)) == -1) {
        perror("UDP Binding error");
        close(tcp_sockfd);
        close(udp_sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming TCP connections
    if (listen(tcp_sockfd, 5) == -1) {
        perror("TCP Listen error");
        close(tcp_sockfd);
        close(udp_sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Receiver is waiting for connections and data...\n");

    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(tcp_sockfd, &read_fds);
        FD_SET(udp_sockfd, &read_fds);

        int max_fd = (tcp_sockfd > udp_sockfd) ? tcp_sockfd : udp_sockfd;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Select error");
            break;
        }

        if (FD_ISSET(tcp_sockfd, &read_fds)) {
            // Handle TCP connection
            int new_sockfd = accept(tcp_sockfd, (struct sockaddr*)&client_addr, &client_len);

            if (new_sockfd == -1) {
                perror("TCP Accept error");
                break;
            }

            printf("Connected to a TCP client.\n");

            while (1) {
                int bytes_received = recv(new_sockfd, received_string, MAX_STRING_LENGTH, 0);

                if (bytes_received <= 0) {
                    // TCP connection closed or error
                    break;
                }

                // Print each character of the received string individually
                for (int i = 0; i < bytes_received; i++) {
                    printf("%c", received_string[i]);
                }
            }

            close(new_sockfd);
            printf("TCP Connection closed.\n");
        }

        if (FD_ISSET(udp_sockfd, &read_fds)) {
            // Handle UDP datagram
            int bytes_received = recvfrom(udp_sockfd, received_string, MAX_STRING_LENGTH, 0, (struct sockaddr*)&client_addr, &client_len);

            if (bytes_received <= 0) {
                perror("UDP Receive error");
                break;
            }

            // Print each character of the received string individually
            for (int i = 0; i < bytes_received; i++) {
                printf("%c", received_string[i]);
            }
        }
    }

    // Close the sockets
    close(tcp_sockfd);
    close(udp_sockfd);
    printf("Receiver program terminated.\n");
    return 0;
}

