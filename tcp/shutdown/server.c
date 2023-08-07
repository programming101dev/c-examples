#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 1) == -1) {
        perror("listen");
        close(sockfd);
        return 1;
    }

    printf("Server listening on port 8080...\n");

    int client_sockfd = accept(sockfd, NULL, NULL);
    if (client_sockfd == -1) {
        perror("accept");
        close(sockfd);
        return 1;
    }

    // Receive data from the client
    char buffer[1024];
    int bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client_sockfd);
        close(sockfd);
        return 1;
    } else if (bytes_received == 0) {
        printf("Client closed the connection.\n");
    } else {
        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);
    }

    // Perform a shutdown on the receiving part of the socket
    shutdown(client_sockfd, SHUT_RD);

    // Wait for the client to close its sending part of the socket
    char temp_buffer[1];
    int status = recv(client_sockfd, temp_buffer, sizeof(temp_buffer), 0);
    if (status == 0) {
        printf("Client closed the sending part of the socket.\n");
    } else {
        perror("recv");
    }

    // Close the sockets
    close(client_sockfd);
    close(sockfd);
    return 0;
}
