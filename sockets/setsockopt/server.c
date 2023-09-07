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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


// TODO: pass the port as a command line option


int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        return 1;
    }

    int optval = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        close(sockfd);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    // Bind the server socket to the specified address and port
    if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Start listening for incoming connections
    if(listen(sockfd, 1) == -1)
    {
        perror("listen");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Server is listening on port 8888...\n");

    // Accept incoming connections
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
    if(client_sockfd == -1)
    {
        perror("accept");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Client connected!\n");

    close(client_sockfd);
    close(sockfd);
    return EXIT_SUCCESS;
}
