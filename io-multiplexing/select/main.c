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
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <unistd.h>


static void setup_signal_handler(void);
static void signal_handler(int signum);
static int socket_create(void);
static void socket_bind(int sockfd, const char *path);
static void socket_close(int sockfd);


#define SOCKET_PATH "/tmp/example_socket" // Replace with your desired socket path


static volatile int running = 1;


int main(void)
{
    int sockfd;
    int *client_sockets = NULL;
    size_t max_clients = 0;
    int max_fd, activity, new_socket, sd;
    int addrlen;
    struct sockaddr_un address;
    fd_set readfds;

    setup_signal_handler();
    unlink(SOCKET_PATH); // Remove the existing socket file if it exists
    sockfd = socket_create();
    socket_bind(sockfd, SOCKET_PATH);

    // Listen for incoming connections
    if(listen(sockfd, SOMAXCONN) == -1)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections...\n");

    while(running)
    {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add the server socket to the set
        FD_SET(sockfd, &readfds);
        max_fd = sockfd;

        // Add the client sockets to the set
        for(size_t i = 0; i < max_clients; i++)
        {
            sd = client_sockets[i];

            if(sd > 0)
            {
                FD_SET(sd, &readfds);
            }

            if(sd > max_fd)
            {
                max_fd = sd;
            }
        }

        // Use select to monitor sockets for read readiness
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if(activity < 0)
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Handle new client connections
        if(FD_ISSET(sockfd, &readfds))
        {
            if((new_socket = accept(sockfd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) == -1)
            {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection established\n");

            // Increase the size of the client_sockets array
            max_clients++;
            client_sockets = (int *)realloc(client_sockets, sizeof(int) * max_clients);
            client_sockets[max_clients - 1] = new_socket;
        }

        // Handle incoming data from existing clients
        for(size_t i = 0; i < max_clients; i++)
        {
            sd = client_sockets[i];

            if(FD_ISSET(sd, &readfds))
            {
                char word_length;
                char word[256];
                ssize_t valread;

                // Receive the word length (uint8_t)
                valread = read(sd, &word_length, sizeof(word_length));
                if(valread <= 0)
                {
                    // Connection closed or error
                    printf("Client %d disconnected\n", sd);
                    close(sd);
                    FD_CLR(sd, &readfds); // Remove the closed socket from the set
                    client_sockets[i] = 0;
                }
                else
                {
                    // Receive the word based on the length received
                    valread = read(sd, word, (size_t) word_length);
                    if(valread <= 0)
                    {
                        // Connection closed or error
                        printf("Client %d disconnected\n", sd);
                        close(sd);
                        FD_CLR(sd, &readfds); // Remove the closed socket from the set
                        client_sockets[i] = 0;
                    }
                    else
                    {
                        // Null-terminate the word and print it
                        word[valread] = '\0';
                        printf("Received word from client %d: %s\n", sd, word);
                    }
                }
            }
        }
    }

    // Cleanup and close all client sockets
    for(size_t i = 0; i < max_clients; i++)
    {
        sd = client_sockets[i];
        if(sd > 0)
        {
            socket_close(sd);
        }
    }

    // Free the client_sockets array
    free(client_sockets);

    // Close the server socket
    socket_close(sockfd);

    // Remove the socket file
    unlink(SOCKET_PATH);

    printf("Server exited successfully.\n");
    return EXIT_SUCCESS;
}


static void setup_signal_handler(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigaction(SIGINT, &sa, NULL);
}


static void signal_handler(int signum)
{
    if(signum == SIGINT)
    {
        running = 0;
    }
}


static int socket_create(void)
{
    int sockfd;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}


static void socket_bind(int sockfd, const char *path)
{
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Bound to domain socket: %s\n", path);
}


static void socket_close(int client_fd)
{
    if (close(client_fd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}
