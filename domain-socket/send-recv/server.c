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
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>


#define SOCKET_PATH "/tmp/example_socket"


// Global flag to indicate if Ctrl+C signal is received
volatile sig_atomic_t exit_flag = 0;


static void sigint_handler(int signum);


int main(void)
{
    int sockfd, client_sockfd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len;
    uint8_t size;
    char word[UINT8_MAX + 1];

    // Register the SIGINT (Ctrl+C) signal handler
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Create a socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Bind the socket to a path
    unlink(SOCKET_PATH); // Remove the existing socket file if it exists
    if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if(listen(sockfd, 5) == -1)
    {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening...\n");

    while(!exit_flag)
    {
        // Accept a client connection
        client_addr_len = sizeof(struct sockaddr_un);
        client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
        if(client_sockfd == -1)
        {
            if(exit_flag)
            {
                // Ignore errors when the server is shutting down
                break;
            }
            perror("accept");
            continue;
        }

        // Read and print words from the client
        while(recv(client_sockfd, &size, sizeof(uint8_t), 0) > 0)
        {
            recv(client_sockfd, word, size, 0);
            word[size] = '\0'; // Null-terminate the string
            printf("Word Size: %u, Word: %s\n", size, word);
        }

        close(client_sockfd);
    }

    close(sockfd);

    // Clean up the socket file before exiting
    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sigint_handler(int signum)
{
    exit_flag = 1;
}

#pragma GCC diagnostic pop
