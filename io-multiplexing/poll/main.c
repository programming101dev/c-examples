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


#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


static void handle_new_client(int server_socket, int **client_sockets, nfds_t *max_clients);
static void handle_client_data(int sd, int **client_sockets, const nfds_t *max_clients);
static void setup_signal_handler(void);
static void sigint_handler(int signum);
static int socket_create(void);
static void socket_bind(int sockfd, const char *path);
static void socket_close(int sockfd);


#define SOCKET_PATH "/tmp/example_socket"
#define MAX_WORD_LEN 256

static volatile sig_atomic_t exit_flag = 0;     // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)


int main(void)
{
    int           *client_sockets;
    nfds_t        max_clients;
    int           num_ready;
    int           sockfd;
    struct pollfd *fds;

    client_sockets = NULL;
    max_clients    = 0;
    fds            = NULL;
    setup_signal_handler();
    unlink(SOCKET_PATH); // Remove the existing socket file if it exists
    sockfd = socket_create();
    socket_bind(sockfd, SOCKET_PATH);

    if(listen(sockfd, SOMAXCONN) == -1)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections on %s...\n", SOCKET_PATH);

    while(!exit_flag)
    {
        struct pollfd *temp_fds;

        temp_fds = (struct pollfd *)realloc(fds, (max_clients + 1) * sizeof(struct pollfd));

        if(temp_fds == NULL)
        {
            perror("Realloc error");
            free(fds);
            exit(EXIT_FAILURE);
        }

        fds = temp_fds;

        // Set up the pollfd structure for the server socket
        fds[0].fd     = sockfd;
        fds[0].events = POLLIN;

        // Set up the pollfd structures for all client sockets
        for(size_t i = 0; i < max_clients; i++)
        {
            int sd;

            sd                = client_sockets[i];
            fds[i + 1].fd     = sd;
            fds[i + 1].events = POLLIN;
        }

        // Use poll to monitor all sockets for activity
        num_ready = poll(fds, max_clients + 1, -1);

        if(num_ready < 0)
        {
            if(errno == EINTR)
            {
                // The poll call was interrupted by a signal (e.g., SIGINT)
                // Continue the loop and retry the poll call
                continue;
            }

            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        // Handle new client connections
        if(fds[0].revents & POLLIN)
        {
            handle_new_client(sockfd, &client_sockets, &max_clients);
        }

        // Handle incoming data from existing clients
        for(size_t i = 0; i < max_clients; i++)
        {
            int sd;

            sd = client_sockets[i];

            if(fds[i + 1].revents & POLLIN)
            {
                handle_client_data(sd, &client_sockets, &max_clients);
            }
        }
    }

    printf("Cleaning up\n");

    // Cleanup and close all client sockets
    for(size_t i = 0; i < max_clients; i++)
    {
        int sd;

        sd = client_sockets[i];

        if(sd > 0)
        {
            socket_close(sd);
        }
    }

    free(client_sockets);
    free(fds);
    socket_close(sockfd);
    unlink(SOCKET_PATH);
    printf("Server exited successfully.\n");

    return EXIT_SUCCESS;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void sigint_handler(int signum)
{
    exit_flag = 1;
}
#pragma GCC diagnostic pop


static void handle_new_client(int server_socket, int **client_sockets, nfds_t *max_clients)
{
    struct sockaddr_un address;
    socklen_t          client_len;
    int                new_socket;

    client_len = sizeof(address);
    new_socket = accept(server_socket, (struct sockaddr *)&address, &client_len);

    if(new_socket == -1)
    {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }

    printf("New connection established\n");

    // Increase the size of the client_sockets array
    (*max_clients)++;
    *client_sockets = (int *)realloc(*client_sockets, sizeof(int) * (*max_clients));

    if(*client_sockets == NULL)
    {
        perror("Realloc error");
        exit(EXIT_FAILURE);
    }

    (*client_sockets)[(*max_clients) - 1] = new_socket;
}


static void handle_client_data(int sd, int **client_sockets, const nfds_t *max_clients)
{
    char    word_length;
    char    word[MAX_WORD_LEN];
    ssize_t valread;

    valread = read(sd, &word_length, sizeof(word_length));

    if(valread <= 0)
    {
        // Connection closed or error
        printf("Client %d disconnected\n", sd);
        close(sd);

        // Mark the disconnected client socket as 0
        for(size_t i = 0; i < *max_clients; i++)
        {
            if((*client_sockets)[i] == sd)
            {
                (*client_sockets)[i] = 0;
                break;
            }
        }
    }
    else
    {
        // Receive the word based on the length received
        valread = read(sd, word, (size_t)word_length);

        if(valread <= 0)
        {
            // Connection closed or error
            printf("Client %d disconnected\n", sd);
            close(sd);

            // Mark the disconnected client socket as 0
            for(size_t i = 0; i < *max_clients; i++)
            {
                if((*client_sockets)[i] == sd)
                {
                    (*client_sockets)[i] = 0;
                    break;
                }
            }
        }
        else
        {
            // Null-terminate the word and print it
            word[valread] = '\0';
            printf("Received word from client %d: %s\n", sd, word);
        }
    }
}


static void setup_signal_handler(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = sigint_handler;
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    sigaction(SIGINT, &sa, NULL);
}


static int socket_create(void)
{
    int sockfd;

#ifdef SOCK_CLOEXEC
    sockfd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
#else
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);   // NOLINT(android-cloexec-socket)
#endif

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

    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Bound to domain socket: %s\n", path);
}


static void socket_close(int sockfd)
{
    if(close(sockfd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}
