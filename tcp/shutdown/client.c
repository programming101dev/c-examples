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
#include <arpa/inet.h>


//TODO: pass port on the command line


int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));

    if(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        perror("connect");
        close(sockfd);
        return 1;
    }

    // Send data over the socket
    char message[] = "Hello, server!";
    send(sockfd, message, sizeof(message), 0);

    // Perform a shutdown on the sending part of the socket
    shutdown(sockfd, SHUT_WR);

    // Receive data from the server
    char buffer[1024];
    int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
    if(bytes_received == -1)
    {
        perror("recv");
        close(sockfd);
        return 1;
    }
    else if(bytes_received == 0)
    {
        printf("Server closed the connection.\n");
    }
    else
    {
        buffer[bytes_received] = '\0';
        printf("Received from server: %s\n", buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
