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

#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static void           setup_signal_handler(void);
static void           sigint_handler(int signum);
static int            socket_create(void);
static void           socket_bind(int sockfd, const char *path);
static void           socket_close(int sockfd);
static struct pollfd *initialize_pollfds(int sockfd, int **client_sockets);
static void           handle_new_connection(int sockfd, int **client_sockets, nfds_t *max_clients, struct pollfd **fds);
static void           handle_client_data(struct pollfd *fds, int *client_sockets, nfds_t *max_clients);
static void           handle_client_disconnection(int **client_sockets, nfds_t *max_clients, struct pollfd **fds, nfds_t client_index);

#define SOCKET_PATH "/tmp/example_socket"
#define MAX_WORD_LEN 256

static volatile sig_atomic_t exit_flag = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

int main(void)
{
    int            sockfd;
    int           *client_sockets = NULL;
    nfds_t         max_clients    = 0;
    struct pollfd *fds;

    setup_signal_handler();
    unlink(SOCKET_PATH);
    sockfd = socket_create();
    socket_bind(sockfd, SOCKET_PATH);

    if(listen(sockfd, SOMAXCONN) == -1)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for incoming connections...\n");
    fds = initialize_pollfds(sockfd, &client_sockets);

    while(!exit_flag)
    {
        int activity;

        activity = poll(fds, max_clients + 1, -1);

        if(activity < 0)
        {
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        // Handle new client connections
        handle_new_connection(sockfd, &client_sockets, &max_clients, &fds);

        if(client_sockets != NULL)
        {
            // Handle incoming data from existing clients
            handle_client_data(fds, client_sockets, &max_clients);
        }
    }

    free(fds);

    // Cleanup and close all client sockets
    for(size_t i = 0; i < max_clients; i++)
    {
        if(client_sockets[i] > 0)
        {
            socket_close(client_sockets[i]);
        }
    }

    free(client_sockets);
    socket_close(sockfd);
    unlink(SOCKET_PATH);
    printf("Server exited successfully.\n");

    return EXIT_SUCCESS;
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sigint_handler(int signum)
{
    exit_flag = 1;
    printf("SIGINT received. Exiting...\n");
}

#pragma GCC diagnostic pop

static int socket_create(void)
{
    int sockfd;

#ifdef SOCK_CLOEXEC
    sockfd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
#else
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);    // NOLINT(android-cloexec-socket)
#endif

    if(sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully.\n");

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

    printf("Socket closed.\n");
}

static struct pollfd *initialize_pollfds(int sockfd, int **client_sockets)
{
    struct pollfd *fds;

    *client_sockets = NULL;

    fds = (struct pollfd *)malloc((1) * sizeof(struct pollfd));

    if(fds == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    fds[0].fd     = sockfd;
    fds[0].events = POLLIN;

    return fds;
}

static void handle_new_connection(int sockfd, int **client_sockets, nfds_t *max_clients, struct pollfd **fds)
{
    if((*fds)[0].revents & POLLIN)
    {
        socklen_t          addrlen;
        int                new_socket;
        int               *temp;
        struct sockaddr_un addr;

        addrlen    = sizeof(addr);
        new_socket = accept(sockfd, (struct sockaddr *)&addr, &addrlen);

        if(new_socket == -1)
        {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }

        (*max_clients)++;
        temp = (int *)realloc(*client_sockets, sizeof(int) * (*max_clients));

        if(temp == NULL)
        {
            perror("realloc");
            free(*client_sockets);
            exit(EXIT_FAILURE);
        }
        else
        {
            struct pollfd *new_fds;
            *client_sockets                       = temp;
            (*client_sockets)[(*max_clients) - 1] = new_socket;

            new_fds = (struct pollfd *)realloc(*fds, (*max_clients + 1) * sizeof(struct pollfd));
            if(new_fds == NULL)
            {
                perror("realloc");
                free(*client_sockets);
                exit(EXIT_FAILURE);
            }
            else
            {
                *fds                        = new_fds;
                (*fds)[*max_clients].fd     = new_socket;
                (*fds)[*max_clients].events = POLLIN;
            }
        }
    }
}

static void handle_client_data(struct pollfd *fds, int *client_sockets, nfds_t *max_clients)
{
    for(nfds_t i = 0; i < *max_clients; i++)
    {
        if(client_sockets[i] != -1 && (fds[i + 1].revents & POLLIN))
        {
            char    word_length;
            ssize_t valread;

            valread = read(client_sockets[i], &word_length, sizeof(word_length));

            if(valread <= 0)
            {
                // Connection closed or error
                printf("Client %d disconnected\n", client_sockets[i]);
                handle_client_disconnection(&client_sockets, max_clients, &fds, i);
            }
            else
            {
                char word[MAX_WORD_LEN];

                valread = read(client_sockets[i], word, (size_t)word_length);

                if(valread <= 0)
                {
                    // Connection closed or error
                    printf("Client %d disconnected\n", client_sockets[i]);
                    handle_client_disconnection(&client_sockets, max_clients, &fds, i);
                }
                else
                {
                    word[valread] = '\0';
                    printf("Received word from client %d: %s\n", client_sockets[i], word);
                }
            }
        }
    }
}

static void handle_client_disconnection(int **client_sockets, nfds_t *max_clients, struct pollfd **fds, nfds_t client_index)
{
    int disconnected_socket = (*client_sockets)[client_index];
    close(disconnected_socket);

    for(nfds_t i = client_index; i < *max_clients - 1; i++)
    {
        (*client_sockets)[i] = (*client_sockets)[i + 1];
    }

    (*max_clients)--;

    for(nfds_t i = client_index + 1; i <= *max_clients; i++)
    {
        (*fds)[i] = (*fds)[i + 1];
    }
}
