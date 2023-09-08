/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */


#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define PORT 8080


// TODO: read the port and hessage from the command line


int main(void)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    char message[] = "Hello, server!";
    int bytes_sent = sendto(sockfd, message, strlen(message), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(bytes_sent == -1)
    {
        perror("sendto");
        close(sockfd);
        return 1;
    }

    char buffer[1024];
    socklen_t server_addr_len = sizeof(server_addr);

    int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &server_addr,
                                  &server_addr_len);
    if(bytes_received == -1)
    {
        perror("recvfrom");
        close(sockfd);
        return 1;
    }

    buffer[bytes_received] = '\0';
    printf("Received from server: %s\n", buffer);

    close(sockfd);
    return 0;
}
